#include "TimeParser.h"

bool TimeParser::isValid(const std::string& input)
{
    if (input.length() != 5) return false;

    bool result = false;
    if ((input[0] - '0') * 10 + (input[1] - '0') < 24 &&
        input[2] == ':' &&
        (input[3] - '0') * 10 + (input[4] - '0') < 60)
    {
        result = true;

        hour = (input[0] - '0') * 10 + (input[1] - '0');
        minute = (input[3] - '0') * 10 + (input[4] - '0');

        return result;
    }
    return result;
}

int TimeParser::getHour() const 
{
    return hour;
}

int TimeParser::getMinute() const 
{
    return minute;
}

