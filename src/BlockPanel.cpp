#include "imgui.h"

#include "BlockPanel.h"

BlockPanel::BlockPanel(Driver& driver) : driver(driver)
{
}

void BlockPanel::render()
{
    ImGui::Text("Placeholder Block");
}

