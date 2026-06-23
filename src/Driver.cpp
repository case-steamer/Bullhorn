#include "TimeParser.h"
#include "Driver.h"

#include <thread>
#include <chrono>
#include <iostream>
#include <string>
#include <algorithm>
#include <mutex>
#include <queue>
#include <stdexcept>


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

std::thread Driver::startPerform()
{
    while (!qBucket.empty())
        qBucket.pop();
    const Queue& queue = xmlio.getQueue();
    std::vector<Queue::BlockEntry> sortedBlocks = queue.allBlocks;
    std::sort(sortedBlocks.begin(), sortedBlocks.end(),
            [](const Queue::BlockEntry& a, const Queue::BlockEntry& b)
            {
            return (a.block.hour * 60 + a.block.minute) < (b.block.hour * 60 + b.block.minute);
            });
    for (const Queue::BlockEntry& entry : sortedBlocks)
    {
        qBucket.push(entry);
    }

    
    auto blockWorker = [&]()
    {
        while (true)
        {
            Queue::BlockEntry next;
            {
                std::lock_guard<std::mutex> qLock(excluder);
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
            std::unique_lock<std::mutex> stopLock(guard);
            auto state = performance_listener.wait_for(stopLock, std::chrono::seconds((int)localParser.secondsUntil()), [this]{return !performance_state;});
            if (state)
                break;
            for (const Block::Track& track : next.block.allTracks)
            {
                if (!performance_state)
                    break;
                if (next.isOverride && audioPlayer.isPlaying())
                {
                    audioPlayer.interrupt();
                }
                audioPlayer.playTrack(track.filepath, track.volume);
            }
        }
    };

    auto func = [blockWorker]()
    {
        std::thread worker1(blockWorker);
        std::thread worker2(blockWorker);
        worker1.join();
        worker2.join();
    };

    performance_state = true;
    onTrigger = [this](){stop();};
    current_mode = PERFORM;
    std::thread returnThread(func);
    return      returnThread;
}

void Driver::addToActiveBlock(const fs::path& path)
{
    if (activeBlockFile.empty())
        return;
    Block::Track track;
    track.filepath = path;
    xmlio.getBlock().allTracks.push_back(track);
    xmlio.writeBlock(activeBlockFile);
}

void Driver::removeTrackFromBlock(int index)
{
    Block& block = xmlio.getBlock();
    if (index < 0 || index >= (int)block.allTracks.size())
        return;
    block.allTracks.erase(block.allTracks.begin() + index);
    xmlio.writeBlock(activeBlockFile);
}

Driver::Mode Driver::getMode()
{
    return current_mode;
}

void Driver::stop()
{
    //need to implement threading so that the gui render() isn't dependent on 
    //the return from executor.join().
    audioPlayer.interrupt();
    auto func = [this]()
    {
        std::unique_lock<std::mutex> stopLock(guard);
        performance_state = false;
        stopLock.unlock();
        performance_listener.notify_all();
        executor.join();
    };
    if (stopper.joinable())
    {
        stopper.join();
    }
    stopper = std::thread(func);
    onTrigger = [this](){executor = startPerform();};
    current_mode = EDIT;
}

void Driver::publicTrigger()
{
    if (!activeQueueFile.empty() || current_mode == PERFORM)
    {
        onTrigger();
    }
    else
    {
        std::runtime_error e("No queue file loaded!");
        message = e.what();
    }
}

