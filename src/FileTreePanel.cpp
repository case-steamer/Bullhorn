//The file tree area should only show directories and supported file types. WAV, MP3, FLAC, M4A/AAC

#include <algorithm>

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
        std::sort(node.subdirs.begin(), node.subdirs.end(), [](const DirNode&a, const DirNode& b)
            {
            return a.path.filename() < b.path.filename();
            }
        );
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void                        FileTreePanel::render()
{
    ImGui::Checkbox("Show hidden files", &showDotFiles);
    ImGui::Separator();

    if(!root.childrenLoaded)
    {
        root.path = defaultDirectory;
        lookIn(root);
    }

    for(DirNode& child : root.subdirs)
    {
        if (!showDotFiles && child.path.filename().string()[0] == '.')
            continue;
        renderNode(child);
    }
}

void                        FileTreePanel::renderNode(DirNode& node)
{
    std::string label = std::string(ICON_FA_FOLDER) + " " + node.path.filename().string();
    if (ImGui::TreeNode(label.c_str()))
    {
        if (!node.childrenLoaded)
            lookIn(node);

        for (DirNode& child : node.subdirs)
        {
            if (!showDotFiles && child.path.filename().string()[0] == '.')
                continue;
            renderNode(child);
        }

        for (const fs::path& file : node.audioFiles)
        {
            if (!showDotFiles && file.filename().string()[0] == '.')
                continue;
            std::string fileLabel = std::string(ICON_FA_MUSIC) + " " + file.filename().string();
            ImGui::Text("%s", fileLabel.c_str());
        }
        
        ImGui::TreePop();
    }
}

