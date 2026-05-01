#pragma once

#include <string>

class TimeParser
{
    public:
        bool isValid(const std::string& input);

    private:
        int hour;
        int minute;
};
