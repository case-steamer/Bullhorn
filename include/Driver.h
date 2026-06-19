#pragma once

#include <queue>
#include <filesystem>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>

#include "TimeParser.h"
#include "FileSysOp.h"
#include "AudioPlayer.h"
#include "XMLIO.h"

namespace fs = std::filesystem;
using WorkQueue = std::queue<Queue::BlockEntry>;

class Driver
{
    public:
        Driver(XMLIO& xmlio);
        FileSysOp   systemAgent;
        XMLIO&      xmlio;
        void        publicTrigger();
        std::thread startPerform();
        void edit();
        fs::path activeBlockFile;
        fs::path activeQueueFile;
        void addToActiveBlock(const fs::path& trackPath);
        void removeTrackFromBlock(int index);
        enum Mode {EDIT = 0, PERFORM};
        Mode getMode();

    private:
        std::string             filepath;
        TimeParser              parser;
        AudioPlayer             audioPlayer;
        Mode                    current_mode = Mode::EDIT;
        std::mutex              guard;
        std::condition_variable performance_listener;
        bool                    performance_state;
        std::thread             executor;
        std::function<void()>   onTrigger = [this](){executor = startPerform();};
        void stop();
        WorkQueue               qBucket;
        std::mutex              excluder;
};
