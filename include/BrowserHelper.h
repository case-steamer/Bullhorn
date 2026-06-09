#pragma once

#include <filesystem>
#include <vector>
#include <stdexcept>
#include <iostream>

#include "IconsFontAwesome6.h"

#include "FileSysOp.h"

namespace fs = std::filesystem;

class BrowserHelper
{
    public:
        BrowserHelper(FileSysOp& validator);
        struct                      DirNode
        {
            fs::path                path;
            bool                    childrenLoaded = false;
            std::vector<DirNode>    subdirs;
            std::vector<fs::path>   filesOfType;
        };

        std::vector<std::string>     validExtensions;
        void                        lookIn(DirNode& node);
        void                        renderNode(DirNode& node);
        bool                        showDotFiles = false;
        DirNode                     root;
        std::string                 defaultDirectory;

    private:
        FileSysOp&                  validator;
};

