#include "TimeParser.h"

bool TimeParser::isValid(std::string input)
{
    if (input.length() != 5) return false;

    bool result = false;
    if ((input[0] - '0') * 10 + (input[1] - '0') < 24 &&
        input[2] == ':' &&
        (input[3] - '0') * 10 + (input[4] - '0') < 60)
    {
        result = true;
        return result;
    }
    return result;
}

