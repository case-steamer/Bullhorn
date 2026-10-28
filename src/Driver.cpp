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
#include <sstream>
#include <iomanip>


Driver::Driver(XMLIO& xmlio) : xmlio(xmlio), systemAgent(*this)
{
}

void Driver::refreshQueue()
{
    while(!activeBlockFile.empty())
    {
        if (!activeProject)
        {
            pushMessage("no queue file loaded!");
            audioPlayer.playBeep();
            return;
        }

        if (systemAgent.isValid(activeBlockFile, xmlio.getQueue()))
        {
            xmlio.initBlock();
            activeBlockFile.clear();
            return;
        }

        Queue::BlockEntry entry;
        entry.filepath      = activeBlockFile;
        entry.block         = xmlio.getBlock();
        entry.isOverride    = false;

        xmlio.addBlock(entry);
        xmlio.writeQueue(activeProject->queue);

        xmlio.initBlock();
        activeBlockFile.clear();
    }
}

std::thread Driver::startPerform()
{
    refreshQueue();
    audioPlayer.engage();
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
            
            std::ostringstream oss;
            oss << std::setw(2) << std::setfill('0') << next.block.hour << 
            ":" << 
            std::setw(2) << std::setfill('0') << next.block.minute;
            std::string scheduledTime = oss.str();
            if (!localParser.isValid(scheduledTime))
                continue;
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

void Driver::removeBlockFromQueue()
{
    xmlio.subtractBlock(activeBlockFile);
}

Driver::Mode Driver::getMode() const
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
    if (activeProject || current_mode == PERFORM)
    {
        onTrigger();
    }
    else
    {
        pushMessage("No queue file loaded!");
    }
}

void Driver::pushMessage(const std::string& message)
{
    msgData data;

    data.message    = message;
    data.timestamp  = std::chrono::steady_clock::now();

    messageDeck.push_back(data);
}

void Driver::destroy()
{
    audioPlayer.interrupt();
    try {
        if (current_mode == PERFORM)
        {
            stop();
        }
        if (stopper.joinable())
        {
            stopper.join();
        }
        if (executor.joinable())
        {
            executor.join();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }
}

Driver::~Driver()
{
    destroy();
}

