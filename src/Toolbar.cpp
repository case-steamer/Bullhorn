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
    auto cursorPositioner = ImGui::GetCursorPos();

    ImVec2 buttonPositioner(ImGui::GetContentRegionAvail().x - 130, ImGui::GetContentRegionAvail().y - 130);    

    ImGui::SetCursorPos(buttonPositioner);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 1.0f, 0.5f, 1.0f));
    if (driver.getMode() == 0)
    {
        if (ImGui::Button("GO", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
        {
            driver.publicTrigger();
        }
    }
    ImGui::PopStyleColor(1);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    if (driver.getMode() == 1)
    {
        if (ImGui::Button("STOP", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
        {
            driver.publicTrigger();
        }
    }
    ImGui::PopStyleColor(1);

    ImVec2 newLinePos(cursorPositioner.x, cursorPositioner.y + 25);
    ImGui::SetCursorPos(newLinePos);

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
            "##Queue File",
            queueBuffer,
            sizeof(queueBuffer),
            ImGuiInputTextFlags_ReadOnly
            );
}

