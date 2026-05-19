#include "XMLIO.h"
#include "Driver.h"
#include <iostream>

int main()
{
    try
    {
        XMLIO xmlio;
        Driver driver(xmlio);

    }
    catch (const std::runtime_error& e)
    {
        std::cout << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
        return 0;
}
