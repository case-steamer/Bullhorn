#include <filesystem>
#include <stdexcept>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <algorithm>

#include "imgui.h"

#include "QueuePanel.h"

QueuePanel::QueuePanel(Driver& driver) : driver(driver)
{
    if (driver.activeProject)
    {
        this->currentPath = driver.activeProject->queue;
    }
}

void QueuePanel::sortBlocks()
    //private method
{
    Queue& queue = driver.xmlio.getQueue();
    auto& blockList = queue.allBlocks;
    std::sort(blockList.begin(), blockList.end(),
            [](const auto &f, const auto &s)
            { 
            if (f.block.hour != s.block.hour)
                {
                    return f.block.hour < s.block.hour;
                }
                return f.block.minute < s.block.minute;
            }
        );
    refreshBuffers();
}

void QueuePanel::refreshBuffers()
    //private method
{
    Queue& queue = driver.xmlio.getQueue();
    if (driver.activeProject)
        currentPath = driver.activeProject->queue;
    timecodeBuffers.clear();
    timecodeFlags.clear();

    for (int i = 0; i < (int)queue.allBlocks.size(); i++)
    {
        auto& blockEntry = queue.allBlocks[i];
        auto hh = blockEntry.block.hour;
        auto mm = blockEntry.block.minute;
        std::array<char, 6> timecodeSpace;
        bool flag = false;
        std::memset(timecodeSpace.data(), 0, sizeof(timecodeSpace));

        std::snprintf(
                timecodeSpace.data(), 
                sizeof(timecodeSpace), 
                "%02d:%02d",
                hh,
                mm
                );
        timecodeBuffers.push_back(timecodeSpace);
        timecodeFlags.push_back(flag);
    }
}

void QueuePanel::render()
    //public method
{
    try
    {
        if (!driver.activeProject)
        {
            ImGui::TextWrapped("Load a project or create a new one to get started.");
        }
        else
        {
            if (std::filesystem::exists(driver.activeProject->queue) && driver.xmlio.getQueue().allBlocks.empty())
            ImGui::TextWrapped("Press 'New Block' to get started with your project!");

            if (pendingSort)
            {
                sortBlocks();
                driver.xmlio.writeQueue(driver.activeProject->queue);
                pendingSort = false;
                clearSelection();
            }
        
            if (currentPath != driver.activeProject->queue)
                refreshBuffers();
            displayQueue();
        }
    }
    catch (const std::runtime_error& e)
    {
        ImGui::Text("%s", e.what());
    }
}


void QueuePanel::displayQueue()
    //public method
{
    Queue& queue = driver.xmlio.getQueue();

    for (int i = 0; i < (int)queue.allBlocks.size(); i++)
    {
        Queue::BlockEntry& currentBlock = queue.allBlocks[i];
        std::string groupLabel = "##track" + std::to_string(i);

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.973f, 0.966f, 0.966f, 1.0f));

        ImGui::BeginChild(groupLabel.c_str(), ImVec2(0, 80), true);

        ImVec2 fieldSize = ImGui::CalcTextSize("HH:MM0");
        std::string timecodeLabel = "##timecode" + std::to_string(i);

        ImGui::SetNextItemWidth(fieldSize.x);
        bool isEntered = ImGui::InputText(
                timecodeLabel.c_str(),
                timecodeBuffers[i].data(),
                timecodeBuffers[i].size(),
                ImGuiInputTextFlags_EnterReturnsTrue
                );
        bool isSelected = (i == selIndex);
        if (isEntered)
        {
            bool passed = true;
            for (Queue::BlockEntry be : queue.allBlocks)
            {
                if (passed)
                {
                    auto hh = be.block.hour;
                    auto mm = be.block.minute;
                    std::array<char, 6> entryTimecode;
                    std::memset(entryTimecode.data(), 0, sizeof(entryTimecode));

                    std::snprintf(
                            entryTimecode.data(),
                            sizeof(entryTimecode),
                            "%02d:%02d",
                            hh,
                            mm
                            );

                    if ((std::strcmp(entryTimecode.data(), timecodeBuffers[i].data()) == 0) && currentBlock.filepath != be.filepath)
                    {
                        driver.audioPlayer.playBeep();
                        driver.pushMessage("Invalid Timecode.");
                        passed = false;
                    }
                }
            }
            if (driver.parser.isValid(timecodeBuffers[i].data()) && passed)
            {
                timecodeFlags[i] = isEntered;
                currentBlock.block.minute = driver.parser.getMinute();
                currentBlock.block.hour = driver.parser.getHour();
                driver.activeBlockFile = currentBlock.filepath;
                driver.xmlio.readBlock(driver.activeBlockFile);
                driver.xmlio.setTime(driver.parser.getHour(), driver.parser.getMinute());
                driver.xmlio.writeBlock(driver.activeBlockFile);
                pendingSort = true;
            }
            else if (!passed)
            {
                pendingSort = true;
            }
        }

        ImGui::SameLine();
        std::string overrideBehavior = "Override##" + std::to_string(i);

        if (ImGui::Checkbox(overrideBehavior.c_str(), &currentBlock.isOverride))
        {
            driver.xmlio.writeQueue(driver.activeProject->queue);
        }

        if (ImGui::Selectable(currentBlock.filepath.filename().string().c_str(), isSelected))
            selIndex = i;
        ImGui::EndChild();
        ImGui::PopStyleColor(1);
    }
}

//public method \/
int QueuePanel::getSelected() const {return selIndex;}

void QueuePanel::clearSelection()
    //public method
{
    selIndex = -1;
}

