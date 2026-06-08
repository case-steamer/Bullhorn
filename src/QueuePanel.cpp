#include "imgui.h"

#include "QueuePanel.h"

QueuePanel::QueuePanel(Driver& driver) : driver(driver)
{
}

void QueuePanel::render()
{
    ImGui::Text("Placeholder Queue");
}

