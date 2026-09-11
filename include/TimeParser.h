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
        int hour = 0;
        int minute = 0;
        double secondsToGo = 0;
};
