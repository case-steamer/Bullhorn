target_sources(Bullhorn PRIVATE
    tinyxml2.cpp
)

target_include_directories(Bullhorn PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

#SDL2 is a system dependency and must be installed before building.
#On Linux: sudo apt install libsdl2-dev
#On Windows: download SDL2 development libraries from https://libsdl.org
#and add to your PATH or specify SDL2_DIR in CMake.

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

include(FetchContent)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY  https://github.com/ocornut/imgui.git
    GIT_TAG         v1.90.4
)

FetchContent_MakeAvailable(imgui)

target_sources(Bullhorn PRIVATE
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp

    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(Bullhorn PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_include_directories(Bullhorn PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(Bullhorn ${SDL2_LIBRARIES})
target_link_libraries(Bullhorn OpenGL::GL)
