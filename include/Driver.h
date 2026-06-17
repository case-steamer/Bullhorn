#pragma once

#include <filesystem>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <fuctional>

#include "TimeParser.h"
#include "FileSysOp.h"
#include "AudioPlayer.h"
#include "XMLIO.h"

namespace fs = std::filesystem;

class Driver
{
    public:
        Driver(XMLIO& xmlio);
        FileSysOp   systemAgent;
        XMLIO&      xmlio;
        std::thread startPerform();
        void edit();
        fs::path activeBlockFile;
        fs::path activeQueueFile;
        void addToActiveBlock(const fs::path& trackPath);
        void removeTrackFromBlock(int index);
        enum getMode;

    private:
        std::string             filepath;
        TimeParser              parser;
        AudioPlayer             audioPlayer;
        enum                    Mode {EDIT = 0, PERFORM};
        std::mutex              guard;
        std::condition_variable performance_listener;
        bool                    performance_state;
        std::thread             executor;
        std::function<void()>   onTrigger;
};
