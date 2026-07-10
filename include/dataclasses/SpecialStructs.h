#pragma once

#include <chrono>
#include <string>

struct msgData
{
    std::string message;
    std::chrono::steady_clock::time_point timestamp;
};

