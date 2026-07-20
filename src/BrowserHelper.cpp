#include <algorithm>

#include "imgui.h"

#include "BrowserHelper.h"

BrowserHelper::BrowserHelper()
{
#ifdef _WIN32
    //TODO Add an iterator to scan for drives for use in Windows.
    defaultDirectory = "C:\\Users";
#else
    //TODO Add logic to automatically open to "/>/home" for Linux.
    defaultDirectory = "/";
#endif
}

void                        BrowserHelper::lookIn(DirNode& node)
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
                else if (this->algoRule(pn.path()))
                {
                    node.filesOfType.push_back(pn.path());
                }
            }
            node.childrenLoaded = true;
        }
        std::sort(node.subdirs.begin(), node.subdirs.end(), [](const DirNode& a, const DirNode& b)
            {
            return a.path.filename() < b.path.filename();
            }
        );
    }
    catch (const std::exception& e)
    {
        this->onError(e.what());
        //std::cerr << e.what() << std::endl;
    }
}

void                        BrowserHelper::renderNode(DirNode& node)
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

        for (fs::path& file : node.filesOfType)
        {
            if (!showDotFiles && file.filename().string()[0] == '.')
                continue;
            std::string fileIcon;
            if (file.extension() == ".xml")
                fileIcon = std::string(ICON_FA_FILE_CODE);
            else
                fileIcon = std::string(ICON_FA_MUSIC);
            std::string fileLabel = fileIcon + " " + file.filename().string();
            if (ImGui::Selectable(fileLabel.c_str()))
                lastSelected = file;
        }
        ImGui::TreePop();
    }
}
            
void BrowserHelper::canGenerate(DirNode& node)
{
    std::string label = std::string(ICON_FA_FOLDER) + " " + node.path.filename().string();
    int flags = ImGuiTreeNodeFlags_OpenOnArrow;
    if (lastSelected == node.path)
    {
        flags |= ImGuiTreeNodeFlags_Selected;
    }
    bool isExpanded = ImGui::TreeNodeEx(label.c_str(), flags);
    if (ImGui::IsItemClicked())
    {
        lastSelected = node.path;
    }
    if (isExpanded)
    {
        if (!node.childrenLoaded)
        lookIn(node);
        for (DirNode& child : node.subdirs)
        {
            if (!showDotFiles && child.path.filename().string()[0] == '.')
                continue;
            canGenerate(child);
        }
    ImGui::TreePop();
    }
}

