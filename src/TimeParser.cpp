#include "TimeParser.h"
#include <ctime>
#include <chrono>

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

bool TimeParser::isValidTime() 
{
    auto    now         =   std::chrono::system_clock::now();
    std::time_t now_time_t =std::chrono::system_clock::to_time_t(now);
    std::tm local_tm;

#ifdef _WIN32
    localtime_s(&local_tm, &now_time_t);
#else
    localtime_r(&now_time_t, &local_tm);
#endif

    std::tm start_of_day = local_tm;
    start_of_day.tm_hour = 0;
    start_of_day.tm_min = 0;
    start_of_day.tm_sec = 0;
    std::time_t start_of_day_time_t = std::mktime(&start_of_day);

    bool    result      =   false;
    double  seconds     =   ((hour * 60) * 60) + (minute * 60);
    double  daySeconds  =   std::difftime(now_time_t, start_of_day_time_t);
    double  secondsToGo =   std::difftime(seconds, daySeconds);

    if (secondsToGo > 0)
    {
        result = true;
        this->secondsToGo = secondsToGo;
        return result;
    }
    return result;
}

double TimeParser::secondsUntil() const
{
    return secondsToGo;
}

int TimeParser::getHour() const
{
    return hour;
}

int TimeParser::getMinute() const 
{
    return minute;
}

