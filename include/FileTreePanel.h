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
        std::vector<std::string> lookIn(const std::string& currentDirectory);
        void render() override;
        void driverImportFile(std::string filepath);

    private:
        Driver&     driver;
        std::string defaultDirectory;
        std::mutex  cdLock;
        std::thread worker;
};
        
