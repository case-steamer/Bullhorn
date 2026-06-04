#pragma once

#include <filesystem>
#include <string>
#include <mutex>
#include <vector>
#include <thread>
#include <stdexcept>
#include <iostream>

#include "IPanel.h"
#include "Driver.h"

namespace fs = std::filesystem;

class FileTreePanel : public IPanel
{
    public:
        FileTreePanel(Driver& driver);
        struct                      DirNode
        {
            fs::path                path;
            bool                    childrenLoaded =    false;
            std::vector<DirNode>    subdirs;
            std::vector<fs::path>   audioFiles;
        };

        void                        lookIn(DirNode& node);
        void                        render() override;
        void                        driverImportFile(std::string filepath);
       
    private:
        Driver&     driver;
        std::string defaultDirectory;
        std::mutex  cdLock;
        std::thread worker;
        DirNode     root;
};
        
