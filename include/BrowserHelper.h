#pragma once

#include <filesystem>
#include <functional>
#include <vector>
#include <stdexcept>
#include <iostream>

#include "IconsFontAwesome6.h"

namespace fs = std::filesystem;

class BrowserHelper
{
    public:
        BrowserHelper();
        struct                      DirNode
        {
            fs::path                path;
            bool                    childrenLoaded = false;
            std::vector<DirNode>    subdirs;
            std::vector<fs::path>   filesOfType;
        };

        std::function<bool(const fs::path&)> algoRule;
        void                        lookIn(DirNode& node);
        void                        renderNode(DirNode& node);
        void                        canGenerate(DirNode& node);
        bool                        showDotFiles = false;
        DirNode                     root;
        std::string                 defaultDirectory;
        fs::path                    lastSelected;

    private:
};

