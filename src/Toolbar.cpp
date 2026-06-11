#include <stdexcept>

#include "imgui.h"

#include "Toolbar.h"

Toolbar::Toolbar(Driver& driver) : driver(driver), blockBrowser(driver.systemAgent), queueBrowser(driver.systemAgent)
{
    blockBrowser.validExtensions = {".xml"};
    blockBrowser.root.path = blockBrowser.defaultDirectory;
    queueBrowser.validExtensions = {".xml"};
    queueBrowser.root.path = queueBrowser.defaultDirectory;
}

void Toolbar::render()
{
    if (ImGui::Button("Open Block File..."))
    {
        blockBrowserOpen = true;

        if (blockBrowserOpen)
            ImGui::OpenPopup("BlockFileBrowser");
    }

    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
    if (ImGui::BeginPopup("BlockFileBrowser"))
    {
        if (!blockBrowser.root.childrenLoaded)
            blockBrowser.lookIn(blockBrowser.root);

        for (auto& child : blockBrowser.root.subdirs)
            blockBrowser.renderNode(child);
        for (const auto& file : blockBrowser.root.filesOfType)
        {
            std::string fileLabel = file.filename().string();
            if (ImGui::Selectable(fileLabel.c_str()))
                blockBrowser.lastSelected = file;
        }

        if (!blockBrowser.lastSelected.empty())
        {
            driver.activeBlockFile = blockBrowser.lastSelected;
            driver.xmlio.readBlock(driver.activeBlockFile);
            blockBrowser.lastSelected.clear();
            ImGui::CloseCurrentPopup();
            blockBrowserOpen = false;
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleColor(2);

    ImGui::SameLine();
    char blockBuffer[512];
    strncpy(blockBuffer, driver.activeBlockFile.string().c_str(), sizeof(blockBuffer));
    ImGui::InputText(
            "Block File",
            blockBuffer,
            sizeof(blockBuffer),
            ImGuiInputTextFlags_ReadOnly
            );

    if (ImGui::Button("Open Queue File..."))
    {
        queueBrowserOpen = true;

        if (queueBrowserOpen)
            ImGui::OpenPopup("QueueFileBrowser");
    }

    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
    if (ImGui::BeginPopup("QueueFileBrowser"))
    {
        if (!queueBrowser.root.childrenLoaded)
            queueBrowser.lookIn(queueBrowser.root);

        for (auto& child : queueBrowser.root.subdirs)
            queueBrowser.renderNode(child);
        for (const auto& file : queueBrowser.root.filesOfType)
        {
            std::string fileLabel = file.filename().string();
            if (ImGui::Selectable(fileLabel.c_str()))
                queueBrowser.lastSelected = file;
        }

        if (!queueBrowser.lastSelected.empty())
        {
            driver.activeQueueFile = queueBrowser.lastSelected;
            driver.xmlio.readQueue(driver.activeQueueFile);
            queueBrowser.lastSelected.clear();
            ImGui::CloseCurrentPopup();
            queueBrowserOpen = false;
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleColor(2);

    ImGui::SameLine();
    char queueBuffer[512];
    strncpy(queueBuffer, driver.activeQueueFile.string().c_str(), sizeof(queueBuffer));
    ImGui::InputText(
            "Block File",
            queueBuffer,
            sizeof(queueBuffer),
            ImGuiInputTextFlags_ReadOnly
            );
}

