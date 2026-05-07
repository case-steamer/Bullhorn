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
        void perform();
        void edit();

    private:
        std::string filepath;
        TimeParser  parser;
        FileSysOp   systemAgent;
        AudioPlayer audioPlayer;
        XMLIO&      xmlio;
};
