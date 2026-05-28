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
        
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            throw std::runtime_error(SDL_GetError());
        }
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        SDL_Window* window = SDL_CreateWindow(
            "Bullhorn",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            1280,
            720,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
        );

        if (window == nullptr)
            throw std::runtime_error(SDL_GetError());

        SDL_GLContext gl_context = SDL_GL_CreateContext(window);
        if (gl_context == nullptr)
            throw std::runtime_error(SDL_GetError());
        
        SDL_GL_MakeCurrent(window, gl_context);
        SDL_GL_SetSwapInterval(1);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
        ImGui_ImplOpenGL3_Init("#version 130");
    }
    catch (const std::runtime_error& e)
    {
        std::cout << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
        return 0;
}
