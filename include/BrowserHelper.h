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

        void                        lookIn(DirNode& node);
        bool                        showDotFiles = false;

    private:
        FileSysOp&      validator;
        std::string     defaultDirectory;
        DirNode         root;
        void            renderNode(DirNode& node);
};

