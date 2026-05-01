#pragma once

#include <string>

class TimeParser
{
    public:
        bool isValid(std::string input);

    private:
        int hour;
        int minute;
};
