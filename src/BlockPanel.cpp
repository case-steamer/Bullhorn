#include <stdexcept>

#include "imgui.h"

#include "BlockPanel.h"

BlockPanel::BlockPanel(Driver& driver) : driver(driver)
{
}

void BlockPanel::render()
{
    try
    {
        displayBlock();
    }
    catch (const std::runtime_error& e)
    {
        ImGui::Text("%s", e.what());
    }
}

void BlockPanel::displayBlock()
{
    const Queue& queue = driver.xmlio.getQueue();
    if (driver.activeProject)
    {
        if (!driver.systemAgent.isValid(driver.activeBlockFile, queue))
            throw std::runtime_error("Active block does not belong to the current queue.");

        Block& block = driver.xmlio.getBlock();

        for (int i = 0; i < (int)block.allTracks.size(); i++)
        {
            Block::Track& track = block.allTracks[i];
            std::string groupLabel = "##track" + std::to_string(i);

            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.973f, 0.966f, 0.966f, 1.0f));

            ImGui::BeginChild(groupLabel.c_str(), ImVec2(0, 60), true);
            bool isSelected = (i == selIndex);
            if (ImGui::Selectable(track.filepath.filename().string().c_str(), isSelected))
                selIndex = i;
            float volumeAsPct = track.volume * 100.0f;
            std::string label = "##vol" + std::to_string(i);
            ImGui::SetNextItemWidth(80.0f);
            if (ImGui::InputFloat(label.c_str(), &volumeAsPct, 0.0f, 0.0f, "%.0f%%"))
            {
                track.volume = volumeAsPct / 100.0f;
                driver.xmlio.writeBlock(driver.activeBlockFile);
            }
            ImGui::EndChild();
            ImGui::PopStyleColor(1);
        }
    }
}

int BlockPanel::getSelected() const {return selIndex;}

void BlockPanel::clearSelection()
{
    selIndex = -1;
}

