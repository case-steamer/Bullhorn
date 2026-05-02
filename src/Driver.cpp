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

    std::string scheduledTime;
    do
    {
        std::cout<< "Enter scheduled time (HH:MM): ";
        std::cin>> scheduledTime;
    } while (!parser.isValid(scheduledTime));

    std::cout<< "Scheduled " << filepath << " to play at " << scheduledTime << std::endl;
}
