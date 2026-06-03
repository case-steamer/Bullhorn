#pragma once

#include <filesystem>
#include <string>
#include <mutex>
#include <vector>
#include <thread>

#include "IPanel.h"
#include "Driver.h"

class FileTreePanel : public IPanel
{
    public:
        FileTreePanel(Driver& driver);
        void render() override;
        std::vector<std::string> lookIn(const std::string& currentDirectory);
        void driverImportFile(std::string filepath);

    private:
        Driver&     driver;
        std::mutex  cdLock;
        std::thread worker;
};
        
