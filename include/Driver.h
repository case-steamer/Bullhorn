#pragma once

#include <string>
#include "TimeParser.h"

class Driver 
{
    public:
        void run();

    private:
        std::string filepath;
        std::string scheduledTime;
        TimeParser parser;
};
