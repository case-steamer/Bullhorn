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
        BrowserHelper   queueBrowser;

    private:
        Driver& driver;
        bool            fileMenuOpen = false;
        bool            nuProjectBrowserOpen = false;
        bool            queueBrowserOpen = false;
        std::vector<std::string>    validExtensions = {".xml"};
        std::string nuProjectContents = "NewProject";
        ImVec2 mousePos;
};
