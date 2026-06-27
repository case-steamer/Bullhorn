#pragma once

#include <string>

class TimeParser
{
    public:
        bool isValid(const std::string& input);
        bool isValidTime();
        int getHour() const;
        int getMinute() const;
        double secondsUntil() const;

    private:
        int hour;
        int minute;
        double secondsToGo;
};
