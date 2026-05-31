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

    ImVec2 available        = ImGui::GetContentRegionAvail();
    float toolbarHeight     = available.y * 0.15f;
    float workAreaHeight    = available.y * 0.70f;
    float notifsHeight      = available.y * 0.15f;
    float colWidth          = available.x * 0.30f;
    float btnColWidth       = available.x * 0.05f;

    ImGui::BeginChild("Toolbar", ImVec2(available.x, toolbarHeight), true);
    ImGui::Text("Toolbar");
    ImGui::EndChild();

    ImGui::BeginChild("File Browser", ImVec2(colWidth, available.y - toolbarHeight), true);
    ImGui::Text("File Browser");
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("RightSide", ImVec2(available.x - colWidth, available.y -                 toolbarHeight), false);
        float rightWidth  = ImGui::GetContentRegionAvail().x;
        float rightHeight = ImGui::GetContentRegionAvail().y;
        float workHeight  = rightHeight * 0.82f;
        float notifHeight = rightHeight * 0.18f;

        ImGui::SetCursorPos(ImVec2(0, 0));
        ImGui::Button(">");
        ImGui::SetCursorPos(ImVec2(0, 30.0f));
        ImGui::Button("<");

        ImGui::SetCursorPos(ImVec2(btnColWidth, 0));
        ImGui::BeginChild("BlockBuilder", ImVec2(colWidth, workHeight), true);
        ImGui::Text("Block Builder");
        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(btnColWidth + colWidth, 0));
        ImGui::Button(">");
        ImGui::SetCursorPos(ImVec2(btnColWidth + colWidth, 30.0f));
        ImGui::Button("<");

        ImGui::SetCursorPos(ImVec2(btnColWidth * 2 + colWidth, 0));
        ImGui::BeginChild("QueueList", ImVec2(colWidth, workHeight), true);
        ImGui::Text("Queue List");
        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(0, workHeight));
        ImGui::BeginChild("Notifications", ImVec2(rightWidth, notifHeight), true);
        ImGui::Text("Notifications");
        ImGui::EndChild();
    
    ImGui::EndChild();

    ImGui::End();
}

