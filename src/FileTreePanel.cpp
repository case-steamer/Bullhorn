//The file tree area should only show directories and supported file types. WAV, MP3, FLAC, M4A/AAC

#include "imgui.h"

#include "FileTreePanel.h"

using fs = std::filesystem;

FileTreePanel::FileTreePanel(Driver& driver) : driver(driver)
{
#ifdef _WIN32
    defaultDirectory = "C:\\Users";
#else
    defaultDirectory = "/home";
#endif
}

std::vector<std::string>    FileTreePanel::lookIn(const std::string& currentDirectory)
{
if (driver.systemAgent.isValid(currentDirectory))
    {
       //iterate over the directory 
    }
}

void                        FileTreePanel::render()
{

}

