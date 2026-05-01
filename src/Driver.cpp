#include "Driver.h"
#include <iostream>

void Driver::run()
{
    std::cout<< "Enter filepath: ";
    std::cin>> filepath;

    do
    {
        std::cout<< "Enter scheduled time (HH:MM): ";
        std::cin>> scheduledTime;
    } while (!parser.isValid(scheduledTime));

    std::cout<< "Scheduled " << filepath << " to play at " << scheduledTime << std::endl;
}
