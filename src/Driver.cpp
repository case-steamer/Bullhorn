#include "Driver.h"
#include <iostream>
#include <string>

void Driver::run()
{
    do
    {
        std::cout<< "Enter filepath: ";
        std::cin>> filepath;
    } while (!systemAgent.isValid(filepath));


    /* TODO: Nancy's graceful handler
     * When running from a pre-built queue XML, missed cues should be skipped silently
     * and the program should roll forward to the next valid upcoming cue automatically.
     * This is in contrast to the current behavior (terminal/build mode) where the user
     * is warned and re-prompted. The queue manager will need to distinguish between
     * these two modes when that feature is implemented.*/

    std::string scheduledTime;
    do
    {
        std::cout<< "Enter scheduled time (HH:MM): ";
        std::cin>> scheduledTime;
        if (!parser.isValid(scheduledTime))
            std::cout<< "Invalid format. Please use HH:MM"<<std::endl;
        else if (!parser.isValidTime())
            std::cout<< "Enter a time in the future."<<std::endl;
    } while (!parser.isValid(scheduledTime) || !parser.isValidTime());

    std::cout<< "Scheduled " << filepath << " to play at " << scheduledTime << std::endl;
}
