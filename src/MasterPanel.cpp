#include <vector>
#include <algorithm>
#include <filesystem>
#include <cstdio>

#include "imgui.h"
#include "IconsFontAwesome6.h"

#include "MasterPanel.h"

namespace fs = std::filesystem;

MasterPanel::MasterPanel(Driver& driver) :
    driver(driver),
    toolbar(driver),
    filePanel(driver),
    blockPanel(driver),
    queuePanel(driver),
    notifBar(driver)
{
}

void MasterPanel::render()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::SetNextWindowBgAlpha(1.0f);
    ImGui::Begin(
            "Bullhorn",
            nullptr,
            ImGuiWindowFlags_NoTitleBar     |
            ImGuiWindowFlags_NoResize       |
            ImGuiWindowFlags_NoMove         |
            ImGuiWindowFlags_NoScrollbar    |
            ImGuiWindowFlags_NoBringToFrontOnFocus
            );

    ImVec2 available        = ImGui::GetContentRegionAvail();
    float toolbarHeight     = available.y * 0.15f;
    float workAreaHeight    = available.y * 0.70f;
    float notifsHeight      = available.y * 0.15f;
    float colWidth          = available.x * 0.30f;

    ImGui::BeginChild("Toolbar", ImVec2(available.x, toolbarHeight), true);
    toolbar.render();
    ImGui::EndChild();

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 0.4f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

    ImGui::BeginChild("File Browser", ImVec2(colWidth, available.y - toolbarHeight), true);
    //ImGui::Text("File Browser");
    filePanel.render();
    ImGui::EndChild();

    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    float spacing = ImGui::GetStyle().ItemSpacing.x;

    ImGui::BeginChild("RightSide", ImVec2(available.x - colWidth - spacing, available.y -                 toolbarHeight), false);
        float rightWidth    = ImGui::GetContentRegionAvail().x;
        float rightHeight   = ImGui::GetContentRegionAvail().y;
        float innerColW     = rightWidth * (30.0f / 70.0f);
        float workHeight    = rightHeight * 0.82f;
        float notifHeight   = rightHeight * 0.18f;
        float btnColWidth   = rightWidth * (5.0f / 70.0f);
        float vOffset       = ImGui::GetFrameHeight() + 120;
        float btnW          = ImGui::CalcTextSize(">>>").x + ImGui::GetStyle().FramePadding.x * 2;
        float centeredX     = (btnColWidth - btnW) / 2.0f;
        float centeredX2    = (btnColWidth + innerColW) + centeredX;

        if (driver.getMode() == Driver::PERFORM)
        {
            ImGui::BeginDisabled();
        }

        ImGui::SetCursorPos(ImVec2(centeredX, vOffset));
        if (ImGui::Button(">>>##toblock"))
        {
            fs::path fileToPush = filePanel.getLastSelected();
            if (!fileToPush.empty())
                driver.addToActiveBlock(fileToPush);
        }
        ImGui::SetCursorPos(ImVec2(centeredX, 30.0f + vOffset));
        if (ImGui::Button("<<<##fromblock"))
        {
            int trackID = blockPanel.getSelected();
            if (trackID >= 0)
            {
                driver.removeTrackFromBlock(trackID);
                blockPanel.clearSelection();
            }
        }
        ImGui::SetCursorPos(ImVec2(centeredX - 18.0f, 60.0f + vOffset));
        if (ImGui::Button("New Block"))
        {
            char blockTag = 'b';
            std::vector<int> blockStems;
            int newID;

            for (Queue::BlockEntry be : driver.xmlio.getQueue(driver.activeQueueFile).allBlocks)
            {
                std::string beStem = be.filepath.stem().string().erase(0, 1);
                int stemNum = std::stoul(beStem);
                blockStems.push_back(stemNum);
            }

            driver.xmlio.initBlock();

            for (int i = 0; i < 1441; i++)
            {
                if (std::find(blockStems.begin(), blockStems.end(), i) == blockStems.end())
                {
                    newID = i;
                    break;
                }
            }
            fs::path placeIn = driver.activeQueueFile.parent_path();
            char buffer[6];
            std::snprintf(buffer, 6, "%c%04d", blockTag, newID);
            std::string idString = std::string(buffer);
            placeIn.append(idString + ".xml");
            driver.xmlio.writeBlock(placeIn);
            driver.activeBlockFile = placeIn;

            Queue::BlockEntry entry;
            entry.filepath = driver.activeBlockFile;
            entry.block = driver.xmlio.getBlock();
            entry.isOverride = false;
            driver.xmlio.addBlock(entry);
            queuePanel.refreshBuffers();
            driver.xmlio.writeQueue(driver.activeQueueFile);
        }

        ImGui::SetCursorPos(ImVec2(btnColWidth, 0));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 0.4f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

        ImGui::BeginChild("BlockBuilder", ImVec2(innerColW, workHeight), true);
        //ImGui::Text("Block Builder");
        blockPanel.render();
        ImGui::EndChild();

        ImGui::PopStyleVar(1);
        ImGui::PopStyleColor(3);

        ImGui::SetCursorPos(ImVec2(centeredX2, vOffset));
        if (ImGui::Button(">>>##toqueue"))
        {
            driver.refreshQueue();
        }
        ImGui::SetCursorPos(ImVec2(centeredX2, 30.0f + vOffset));
        if (ImGui::Button("<<<##fromqueue"))
        {
            int blockID = queuePanel.getSelected();
            if (blockID >= 0)
            {
                const Queue& queue = driver.xmlio.getQueue();
                if (blockID < (int)queue.allBlocks.size())
                {
                    driver.activeBlockFile = queue.allBlocks[blockID].filepath;
                    driver.xmlio.readBlock(driver.activeBlockFile);
                }
            }
        }
        ImGui::SetCursorPos(ImVec2(centeredX2 + 5.0f, 60.0f + vOffset));
        if (ImGui::Button(ICON_FA_TRASH "##discardBlock"))
        {
            int blockID = queuePanel.getSelected();
            if (blockID >= 0)
            {
                const Queue& queue = driver.xmlio.getQueue();
                if (blockID < (int)queue.allBlocks.size())
                {
                    driver.activeBlockFile = queue.allBlocks[blockID].filepath;
                    driver.xmlio.subtractBlock(driver.activeBlockFile);
                    queuePanel.refreshBuffers();
                    queuePanel.clearSelection();
                    driver.systemAgent.deleteFile(driver.activeBlockFile);
                }
            }
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("discard selected block");
        }

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 0.4f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

        ImGui::SetCursorPos(ImVec2(btnColWidth * 2 + innerColW, 0));
        ImGui::BeginChild("QueueList", ImVec2(innerColW, workHeight), true);
        queuePanel.render();
        ImGui::EndChild();

        ImGui::PopStyleVar(1);
        ImGui::PopStyleColor(3);

        if (driver.getMode() == Driver::PERFORM)
        {
            ImGui::EndDisabled();
        }

        ImGui::SetCursorPos(ImVec2(0, workHeight));
        ImGui::BeginChild("Notifications", ImVec2(rightWidth, notifHeight), true);
        notifBar.render();
        ImGui::EndChild();
    
    ImGui::EndChild();

    ImGui::End();
}

