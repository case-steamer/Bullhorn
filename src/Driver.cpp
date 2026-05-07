#include "Driver.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <string>

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
    } while (!systemAgent.isValid(filepath));


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
    xmlio.writeBlock();

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
    for (Queue::BlockEntry blockEntry : queue.allBlocks)
    {
        xmlio.initBlock();
        xmlio.readBlock(blockEntry.filepath);
        const Block& block = xmlio.getBlock();
        std::string scheduledTime = std::to_string(block.hour) + ":" + std::to_string(block.minute);
        parser.isValid(scheduledTime);
        if (!parser.isValidTime())
        {
            continue;
        }
        std::this_thread::sleep_for(std::chrono::seconds((int)parser.secondsUntil()));
        for (const Block::Track& track : block.allTracks)
        {
            audioPlayer.playTrack(track.filepath);
        }
    }
}

