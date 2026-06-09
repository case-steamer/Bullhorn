//The file tree area should only show directories and supported file types. WAV, MP3, FLAC, M4A/AAC

#include <algorithm>

#include "imgui.h"

#include "FileTreePanel.h"

namespace fs = std::filesystem;

FileTreePanel::FileTreePanel(Driver& driver) : 
    driver(driver), 
    helper(driver.systemAgent)
{
    helper.validExtensions = {".mp3", ".wav", ".flac", ".m4a"};
}

void    FileTreePanel::render()
{
    ImGui::Checkbox("Show hidden files", &helper.showDotFiles);
    ImGui::Separator();

    if(!helper.root.childrenLoaded)
    {
        helper.root.path = helper.defaultDirectory;
        helper.lookIn(helper.root);
    }

    helper.renderNode(helper.root);
}

