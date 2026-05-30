#include "imgui.h"

#include "MasterPanel.h"

MasterPanel::MasterPanel(Driver& driver) : driver(driver)
{
}

void MasterPanel::render()
{
    ImGui::Begin("Bullhorn");
    ImGui::Text("Master Panel");
    ImGui::End();
}

