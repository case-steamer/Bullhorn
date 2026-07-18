#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "imgui.h"

#include "IPanel.h"
#include "Driver.h"
#include "BrowserHelper.h"

namespace fs = std::filesystem;

class Toolbar : public IPanel
{
    public:
        Toolbar(Driver& driver);
        void render() override;

        BrowserHelper   nuProjectBrowser;
        bool            nuProjectBrowserOpen = false;

        BrowserHelper   queueBrowser;
        bool            queueBrowserOpen = false;

    private:
        Driver& driver;
        std::vector<std::string>    validExtensions = {".xml"};
        std::string nuProjectContents = "NewProject";
        ImVec2 mousePos;
};
