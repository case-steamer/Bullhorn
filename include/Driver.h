#pragma once

#include <queue>
#include <deque>
#include <filesystem>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>
#include <optional>

#include "TimeParser.h"
#include "FileSysOp.h"
#include "AudioPlayer.h"
#include "XMLIO.h"
#include "SpecialStructs.h"
#include "Project.h"

namespace fs = std::filesystem;
using WorkQueue = std::queue<Queue::BlockEntry>;

class Driver
{
    public:
        Driver(XMLIO& xmlio);
        FileSysOp   systemAgent;
        XMLIO&      xmlio;
        AudioPlayer audioPlayer;
        TimeParser  parser;
        void        publicTrigger();
        std::thread startPerform();
        void        refreshQueue();
        fs::path    activeBlockFile;
        std::optional<Project>    activeProject;
        void        addToActiveBlock(const fs::path& trackPath);
        void        removeTrackFromBlock(int index);
        void        removeBlockFromQueue();
        enum        Mode {EDIT = 0, PERFORM};
        Mode        getMode();
        std::deque<msgData> messageDeck;
        void        pushMessage(std::string message);

    private:
        std::string             filepath;
        Mode                    current_mode = Mode::EDIT;
        std::mutex              guard;
        std::condition_variable performance_listener;
        bool                    performance_state;
        std::thread             executor;
        std::thread             stopper;
        std::function<void()>   onTrigger = [this](){executor = startPerform();};
        void stop();
        WorkQueue               qBucket;
        std::mutex              excluder;
};
