#pragma once

#include <filesystem>
#include <string>

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
        void perform();
        void edit();
        fs::path activeBlockFile;
        fs::path activeQueueFile;
        void addToActiveBlock(const fs::path& trackPath);

    private:
        std::string filepath;
        TimeParser  parser;
        AudioPlayer audioPlayer;
};
