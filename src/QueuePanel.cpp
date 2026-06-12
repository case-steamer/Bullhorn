#include <stdexcept>

#include "imgui.h"

#include "QueuePanel.h"

QueuePanel::QueuePanel(Driver& driver) : driver(driver)
{
}

void QueuePanel::render()
{
    try
    {
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

        ImGui::BeginChild(groupLabel.c_str(), ImVec2(0, 60), true);
        ImGui::Text("%s", currentBlock.filepath.filename().string().c_str());
        ImGui::EndChild();
        ImGui::PopStyleColor(1);
    }
}

