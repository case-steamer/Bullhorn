#include "TimeParser.h"
#include "Driver.h"

#include <thread>
#include <chrono>
#include <iostream>
#include <string>
#include <algorithm>
#include <mutex>
#include <queue>

using WorkQueue = std::queue<Queue::BlockEntry>;

Driver::Driver(XMLIO& xmlio) : xmlio(xmlio)
{
}

void Driver::edit()
{
    xmlio.initBlock();
    do
    {
        std::cout<< "Enter filepath: ";
        std::cin>> filepath;
    } while (!systemAgent.isValid(fs::path(filepath)));


    /* TODO: Nancy's graceful handler
     * When running from a pre-built queue XML, missed cues should be skipped silently
     * and the program should roll forward to the next valid upcoming cue automatically.
     * This is in contrast to the current behavior (terminal/build mode) where the user
     * is warned and re-prompted. The queue manager will need to distinguish between
     * these two modes when that feature is implemented.*/

    std::string scheduledTime;
    do
    {
        std::cout<< "Enter scheduled time (HH:MM): ";
        std::cin>> scheduledTime;
        if (!parser.isValid(scheduledTime))
            std::cout<< "Invalid format. Please use HH:MM"<<std::endl;
    } while (!parser.isValid(scheduledTime) || !parser.isValidTime());

    xmlio.setTime(parser.getHour(), parser.getMinute());
    xmlio.addData(filepath);
    xmlio.writeBlock(activeBlockFile);

    Queue::BlockEntry entry;
    entry.filepath = "/home/case_steamer/CPP_Projects/LearnDependencies/Bullhorn/test-assets/test_block.xml";
    entry.isOverride = false;
    xmlio.addBlock(entry);
    xmlio.writeQueue();
}

void Driver::perform()
{
    xmlio.initQueue();
    xmlio.readQueue("/home/case_steamer/CPP_Projects/LearnDependencies/Bullhorn/test-assets/test_queue.xml");
    const Queue& queue = xmlio.getQueue();
    std::vector<Queue::BlockEntry> sortedBlocks = queue.allBlocks;
    std::sort(sortedBlocks.begin(), sortedBlocks.end(),
            [](const Queue::BlockEntry& a, const Queue::BlockEntry& b)
            {
            return (a.block.hour * 60 + a.block.minute) < (b.block.hour * 60 + b.block.minute);
            });
    WorkQueue qBucket;
    for (const Queue::BlockEntry& entry : sortedBlocks)
    {
        qBucket.push(entry);
    }

    std::mutex excluder;
    
    auto blockWorker = [&]()
    {
        while (true)
        {
            Queue::BlockEntry next;
            {
                std::lock_guard<std::mutex> guard(excluder);
                if (qBucket.empty()) break;
                next = qBucket.front();
                qBucket.pop();
            }

            TimeParser localParser;
            
            std::string scheduledTime = std::to_string(next.block.hour) + ":" + std::to_string(next.block.minute);
            localParser.isValid(scheduledTime);
            if (!localParser.isValidTime())
            {
             continue;
            }
            std::this_thread::sleep_for(std::chrono::seconds((int)localParser.secondsUntil()));
            for (const Block::Track& track : next.block.allTracks)
            {
                if (next.isOverride && audioPlayer.isPlaying())
                {
                    audioPlayer.interrupt();
                }
                audioPlayer.playTrack(track.filepath, track.volume);
            }
        }
    };
    std::thread worker1(blockWorker);
    std::thread worker2(blockWorker);
    worker1.join();
    worker2.join();
}

