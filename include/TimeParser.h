#pragma once

#include <string>

class TimeParser
{
    public:
        bool isValid(const std::string& input);
        int getHour() const;
        int getMinute() const;

    private:
        int hour;
        int minute;
};
