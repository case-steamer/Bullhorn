#pragma once

#include <mutex>
#include <vector>
#include <thread>

#include "IconsFontAwesome6.h"

#include "IPanel.h"
#include "BrowserHelper.h"
#include "Driver.h"

namespace fs = std::filesystem;

class FileTreePanel : public IPanel
{
    public:
        FileTreePanel(Driver& driver);
        void            render() override;
        void            driverImportFile(std::string filepath);
       
    private:
        Driver&         driver;
        BrowserHelper   helper;
        bool            showDotFiles = false;
};
        
