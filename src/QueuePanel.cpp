#include <stdexcept>
#include <cstring>
#include <cstdio>

#include "imgui.h"

#include "QueuePanel.h"

QueuePanel::QueuePanel(Driver& driver) : driver(driver)
{
}

void QueuePanel::refreshBuffers()
{
    if (currentPath != driver.activeQueueFile)
    {
        Queue& queue = driver.xmlio.getQueue();
        currentPath = driver.activeQueueFile;
        timecodeBuffers.clear();
        for (int i = 0; i < (int)queue.allBlocks.size(); i++)
        {
            auto& blockEntry = queue.allBlocks[i];
            auto hh = blockEntry.block.hour;
            auto mm = blockEntry.block.minute;
            std::array<char, 6> timecodeSpace;
            std::memset(timecodeSpace.data(), 0, sizeof(timecodeSpace));

            std::snprintf(
                    timecodeSpace.data(), 
                    sizeof(timecodeSpace), 
                    "%02d:%02d",
                    hh,
                    mm
                    );

            timecodeBuffers.push_back(timecodeSpace);
        }
    }
}

void QueuePanel::render()
{
    try
    {
        refreshBuffers();
        displayQueue();
    }
    catch (const std::runtime_error& e)
    {
        ImGui::Text("%s", e.what());
    }
}

void QueuePanel::displayQueue()
{
    Queue& queue = driver.xmlio.getQueue();

    for (int i = 0; i < (int)queue.allBlocks.size(); i++)
    {
        Queue::BlockEntry currentBlock = queue.allBlocks[i];
        std::string groupLabel = "##track" + std::to_string(i);

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.973f, 0.966f, 0.966f, 1.0f));

        ImGui::BeginChild(groupLabel.c_str(), ImVec2(0, 80), true);

        ImVec2 fieldSize = ImGui::CalcTextSize("HH:MM0");
        std::string timecodeLabel = " ";

        ImGui::SetNextItemWidth(fieldSize.x);
        ImGui::InputText(
                timecodeLabel.c_str(),
                timecodeBuffers[i].data(),
                timecodeBuffers[i].size(),
                ImGuiInputTextFlags_ReadOnly
                );
        bool isSelected = (i == selIndex);
        if (ImGui::Selectable(currentBlock.filepath.filename().string().c_str(), isSelected))
            selIndex = i;
        ImGui::EndChild();
        ImGui::PopStyleColor(1);
    }
}

int QueuePanel::getSelected() const {return selIndex;}

void QueuePanel::clearSelection()
{
    selIndex = -1;
}

