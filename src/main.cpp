#include "XMLIO.h"
#include "Driver.h"
#include <iostream>

int main()
{
    XMLIO xmlio;
    Driver driver(xmlio);

    int mode = 0;
    do
    {
        std::cout<< "Select mode: (1) Edit or (2) Perform ";
        std::cin >> mode;
    } while (mode != 1 && mode !=2);

    (mode == 1) ? driver.edit() : driver.perform();

    return 0;
}
