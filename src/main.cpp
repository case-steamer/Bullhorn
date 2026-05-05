#include "XMLIO.h"
#include "Driver.h"

int main()
{
    XMLIO xmlio;
    Driver driver(xmlio);
    driver.run();

    return 0;
}
