#include "Driver.h"
#include <iostream>

void Driver::run()
{
    std::cout<< "Enter filepath: ";
    std::cin>> filepath;

    std::cout<< "Enter scheduled time (HH:MM): ";
    std::cin>> scheduledTime;

    std::cout<< "Scheduled " << filepath << " to play at " << scheduledTime << std::endl;
}
