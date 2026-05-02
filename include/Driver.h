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
        TimeParser  parser;
        FileSysOp   systemAgent;
};
