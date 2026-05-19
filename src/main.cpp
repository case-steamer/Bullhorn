#include <SDL.h>
#include <SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
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
