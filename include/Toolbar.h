#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "IPanel.h"
#include "Driver.h"
#include "BrowserHelper.h"

namespace fs = std::filesystem;

class Toolbar : public IPanel
{
    public:
        Toolbar(Driver& driver);
        void render() override;
        BrowserHelper   blockBrowser;
        bool            blockBrowserOpen = false;
        
    private:
        Driver& driver;
};
