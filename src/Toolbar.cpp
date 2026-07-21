#include <stdexcept>
#include <cctype>

#include "imgui.h"

#include "Toolbar.h"

Toolbar::Toolbar(Driver& driver) : driver(driver), nuProjectBrowser(), queueBrowser()
{
    nuProjectBrowser.algoRule = [this](const fs::path& path)
    {
        return false;
    };

    nuProjectBrowser.onError = [this](const std::string& msg)
    {
        this->driver.pushMessage(msg);
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

    queueBrowser.onError = [this](const std::string& msg)
    {
        this->driver.pushMessage(msg);
    };

    nuProjectBrowser.root.path = nuProjectBrowser.defaultDirectory;
    queueBrowser.root.path = queueBrowser.defaultDirectory;
}

void Toolbar::render()
{
    const auto rule       = ImGui::GetContentRegionAvail();
    const auto ruleHeight = rule.y;
    const auto viewport   = ImGui::GetMainViewport();
    const auto vHeight    = viewport->WorkSize.y;

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
            if (ImGui::Button("File"))
            {
                fileMenuOpen = true;
            }
            if (fileMenuOpen)
            {
                ImGui::OpenPopup("File");
            }
     
            if (ImGui::BeginPopup("File"))
            {
                if (ImGui::MenuItem("New Project"))
                {
                    nuProjectBrowserOpen = true;
                    fileMenuOpen = false;
                }
                if (ImGui::MenuItem("Open Project"))
                {
                    queueBrowserOpen = true;
                    fileMenuOpen = false;
                }
                fileMenuOpen = false;
                ImGui::EndPopup();
            }


            if (queueBrowserOpen)
            {
                ImGui::OpenPopup("QueueFileBrowser");
                queueBrowserOpen = false;
            }
            if (nuProjectBrowserOpen)
            {
                mousePos = ImGui::GetMousePos();
                ImGui::OpenPopup("newProjectBrowser");
                nuProjectBrowser.lastSelected.clear();
                nuProjectBrowserOpen = false;
            }
            ImGui::TableNextColumn();
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
     
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
                    //TODO: set browser to look for valid projects instead of QUEUE.xml files.
                    driver.activeQueueFile = queueBrowser.lastSelected;
                    //WARNING: line 130 as is will soon become VERY unsafe!!!!!
                    driver.activeProjectFile = driver.activeQueueFile.parent_path();
                    driver.xmlio.readQueue(driver.activeQueueFile);
                    queueBrowser.lastSelected.clear();
                    ImGui::CloseCurrentPopup();
                    queueBrowserOpen = false;
                }
                ImGui::EndPopup();
            }
   
            ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, vHeight));
            if (ImGui::BeginPopup("newProjectBrowser"))
            {
                const auto browserConstraints = ImVec2(0, ((vHeight / 1.5) - mousePos.y) - (ImGui::GetFrameHeightWithSpacing() * 2));
                ImGui::BeginChild("innerBrowser", browserConstraints);
                    if (!nuProjectBrowser.root.childrenLoaded)
                        nuProjectBrowser.canGenerate(nuProjectBrowser.root);
         
                    for (auto& child : nuProjectBrowser.root.subdirs)
                        nuProjectBrowser.canGenerate(child);
                ImGui::EndChild();
     
                
                char projectBuffer[40];
                strncpy(projectBuffer, nuProjectContents.c_str(), sizeof(projectBuffer));
                projectBuffer[sizeof(projectBuffer) - 1] = 0;
                if (ImGui::InputText(
                        "##Project File",
                        projectBuffer,
                        sizeof(projectBuffer)
                        ))
                {
                    const std::string strP(projectBuffer);
                    nuProjectContents   = strP;
                }
                if (ImGui::Button("Generate New"))
                {
                    if (nuProjectBrowser.lastSelected.empty())
                    {
                        driver.audioPlayer.playBeep();
                        driver.pushMessage("SELECT A DIRECTORY FOR YOUR PROJECT");
                        ImGui::CloseCurrentPopup();
                    }
                    else
                    {
                        driver.activeProjectFile = nuProjectBrowser.lastSelected/nuProjectContents;
                        if (driver.systemAgent.createNewProject())
                        {
                            nuProjectContents = "NewProject";
                            driver.activeQueueFile = driver.activeProjectFile/"QUEUE.xml";
                            driver.xmlio.initQueue();
                            driver.xmlio.writeQueue(driver.activeQueueFile);
                            driver.xmlio.readQueue(driver.activeQueueFile);
                        }
                        else
                        {
                            driver.systemAgent.deleteFile(driver.activeProjectFile);
                            driver.pushMessage("Could not create project. Deleting artifacts.");
                            driver.pushMessage("Done!");
                        }
                        ImGui::CloseCurrentPopup();
                    }
                }
                ImGui::EndPopup();
            }
            ImGui::PopStyleColor(2);
    
            ImGui::TableNextColumn();
    
            char queueBuffer[512];
            strncpy(queueBuffer, driver.activeQueueFile.string().c_str(), sizeof(queueBuffer));
            queueBuffer[sizeof(queueBuffer) - 1] = 0;
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

