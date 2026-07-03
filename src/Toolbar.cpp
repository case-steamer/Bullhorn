#include <stdexcept>
#include <cctype>

#include "imgui.h"

#include "Toolbar.h"

Toolbar::Toolbar(Driver& driver) : driver(driver), blockBrowser(), queueBrowser()
{
    blockBrowser.algoRule = [this](const fs::path& path)
    {
        if (this->driver.systemAgent.isValid(path, this->validExtensions))
        {
            auto exStemSion = path.stem().string();
            if (exStemSion.size() != 5)
                return false;
            if (exStemSion.at(0) != 'b')
                return false;
            for (int i = 1; i < 5; i++)
            {
                if (!std::isdigit(exStemSion.at(i)))
                    return false;
            }
            return true;
        }
        return false;
    };

    queueBrowser.algoRule = [this](const fs::path& path)
    {
        if (this->driver.systemAgent.isValid(path, this->validExtensions))
        {
            if (path.filename() == "QUEUE.xml")
                return true;
            else
                return false;
        }
        return false;
    };

    blockBrowser.root.path = blockBrowser.defaultDirectory;
    queueBrowser.root.path = queueBrowser.defaultDirectory;
}

void Toolbar::render()
{
    auto rule       = ImGui::GetContentRegionAvail();
    auto ruleHeight = rule.y;

    if (!(ImGui::BeginTable("##Toolbar", 2, 0, rule)))
    {
        return;
    }

    ImGui::TableSetupColumn("##HeadColumn1", ImGuiTableColumnFlags_WidthStretch, 0, 0);
    ImGui::TableSetupColumn("##HeadColumn2", ImGuiTableColumnFlags_WidthFixed, ruleHeight, 0);

    ImGui::TableNextRow(0, ruleHeight);

    ImGui::TableNextColumn();
    // Nested table with file browser & etc lives here.
        if(ImGui::BeginTable("##Tools", 2))
        {
            ImGui::TableSetupColumn("##Column1", ImGuiTableColumnFlags_WidthFixed, 0, 0);
            ImGui::TableSetupColumn("##Column2", ImGuiTableColumnFlags_WidthStretch, 0, 0);
            ImGui::TableNextRow(0, 32);
            ImGui::TableNextColumn();
            ImGui::TableNextColumn();
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
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
    
            ImGui::TableNextColumn();
    
            char queueBuffer[512];
            strncpy(queueBuffer, driver.activeQueueFile.string().c_str(), sizeof(queueBuffer));
            ImGui::InputText(
                    "##Queue File",
                    queueBuffer,
                    sizeof(queueBuffer),
                    ImGuiInputTextFlags_ReadOnly
                    );
            ImGui::EndTable();
            }
    
    ImGui::TableNextColumn();
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

    ImGui::EndTable();
}

