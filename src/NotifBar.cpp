#include <string>
#include <chrono>

#include "imgui.h"

#include "NotifBar.h"
#include "SpecialStructs.h"

NotifBar::NotifBar(Driver& driver) : driver(driver)
{
}


void NotifBar::render()
{
    scanMessages();
}

void NotifBar::scanMessages()
{
    if (!driver.messageDeck.empty())
    {
        for (msgData md : driver.messageDeck)
        {
            ImGui::TextUnformatted(md.message.c_str());
        }
    
        if ((std::chrono::steady_clock::now() - driver.messageDeck.front().timestamp) > std::chrono::seconds(5))
        {
            driver.messageDeck.pop_front();
        }
    }
}

