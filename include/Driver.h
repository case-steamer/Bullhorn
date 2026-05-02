#pragma once

#include <string>
#include "TimeParser.h"
#include "FileSysOp.h"
#include "AudioPlayer.h"

class Driver 
{
    public:
        void run();

    private:
        std::string filepath;
        TimeParser  parser;
        FileSysOp   systemAgent;
        AudioPlayer audioPlayer;
};
