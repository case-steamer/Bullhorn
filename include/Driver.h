#pragma once

#include <string>
#include "TimeParser.h"
#include "FileSysOp.h"
#include "AudioPlayer.h"
#include "XMLIO.h"

class Driver
{
    public:
        Driver(XMLIO& xmlio);
        void run();

    private:
        std::string filepath;
        TimeParser  parser;
        FileSysOp   systemAgent;
        AudioPlayer audioPlayer;
        XMLIO&      xmlio;
};
