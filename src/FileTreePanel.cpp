//The file tree area should only show directories and supported file types. WAV, MP3, FLAC, M4A/AAC

#include "imgui.h"

#include "FileTreePanel.h"

namespace fs = std::filesystem;

FileTreePanel::FileTreePanel(Driver& driver) : driver(driver)
{
#ifdef _WIN32
    defaultDirectory = "C:\\Users";
#else
    defaultDirectory = "/home";
#endif
}

void                        FileTreePanel::lookIn(DirNode& node)
{
    try
    {
        if (fs::is_directory(node.path))
        {
            for (auto pn : fs::directory_iterator(node.path))
            {
                if (fs::is_directory(pn.path()))
                {
                    DirNode nuDN;
                    nuDN.path = pn.path();
                    node.subdirs.push_back(nuDN);
                }
                else if (driver.systemAgent.isValid(pn.path()))
                {
                    node.audioFiles.push_back(pn.path());
                }
            }
            node.childrenLoaded = true;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void                        FileTreePanel::render()
{

}

