#include "imgui.h"

#include "MasterPanel.h"

MasterPanel::MasterPanel(Driver& driver) :
    driver(driver),
    filePanel(driver),
    blockPanel(driver)
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
    ImGui::Text("Toolbar");
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

        ImGui::SetCursorPos(ImVec2(centeredX, vOffset));
        ImGui::Button(">>>");
        ImGui::SetCursorPos(ImVec2(centeredX, 30.0f + vOffset));
        ImGui::Button("<<<");

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
        ImGui::Button(">>>");
        ImGui::SetCursorPos(ImVec2(centeredX2, 30.0f + vOffset));
        ImGui::Button("<<<");

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 0.4f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

        ImGui::SetCursorPos(ImVec2(btnColWidth * 2 + innerColW, 0));
        ImGui::BeginChild("QueueList", ImVec2(innerColW, workHeight), true);
        ImGui::Text("Queue List");
        ImGui::EndChild();

        ImGui::PopStyleVar(1);
        ImGui::PopStyleColor(3);

        ImGui::SetCursorPos(ImVec2(0, workHeight));
        ImGui::BeginChild("Notifications", ImVec2(rightWidth, notifHeight), true);
        ImGui::Text("Notifications");
        ImGui::EndChild();
    
    ImGui::EndChild();

    ImGui::End();
}

