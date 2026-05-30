#include "imgui.h"

#include "MasterPanel.h"

MasterPanel::MasterPanel(Driver& driver) : driver(driver)
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
    ImGui::Text("Master Panel");
    ImGui::End();
}

