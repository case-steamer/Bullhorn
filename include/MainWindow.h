#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "Driver.h"

class MainWindow
{
    public:
        MainWindow(Driver& driver);
        void run();
        ~MainWindow();
    
    private:
        SDL_Window* window;
        SDL_GLContext glContext;
        Driver& driver;
};

