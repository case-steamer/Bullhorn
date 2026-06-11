#include "imgui.h"

#include "BlockPanel.h"

BlockPanel::BlockPanel(Driver& driver) : driver(driver)
{
}

void BlockPanel::render()
{
    ImGui::Text("Placeholder Block");
}

void BlockPanel::displayBlock()
{
    const Queue& queue = driver.xmlio.getQueue();
    if (!driver.systemAgent.isValid(driver.activeBlockFile, queue))
        throw std::runtime_error("Active block does not belong to the current queue.");

    const Block& block = driver.xmlio.getBlock(driver.activeBlockFile);
}

