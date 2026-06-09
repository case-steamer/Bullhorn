#include "imgui.h"

#include "Toolbar.h"

Toolbar::Toolbar(Driver& driver) : driver(driver), blockBrowser(driver.systemAgent)
{
    blockBrowser.validExtensions = {".xml"};
    blockBrowser.root.path = blockBrowser.defaultDirectory;
}

void Toolbar::render()
{
    if (ImGui::Button("Open Block File..."))
        blockBrowserOpen = true;

    if (blockBrowserOpen)
        ImGui::OpenPopup("BlockFileBrowser");

    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
    if (ImGui::BeginPopup("BlockFileBrowser"))
    {
        if (!blockBrowser.root.childrenLoaded)
            blockBrowser.lookIn(blockBrowser.root);

        blockBrowser.renderNode(blockBrowser.root);

        if (!blockBrowser.lastSelected.empty())
        {
            driver.activeBlockFile = blockBrowser.lastSelected;
            blockBrowser.lastSelected.clear();
            ImGui::CloseCurrentPopup();
            blockBrowserOpen = false;
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleColor(2);

}

