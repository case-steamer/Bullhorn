#pragma once

#include <string>
#include "TimeParser.h"
#include "FileSysOp.h"

class Driver 
{
    public:
        void run();

    private:
        std::string filepath;
        std::string scheduledTime;
        TimeParser  parser;
        FileSysOp   systemAgent;
};
