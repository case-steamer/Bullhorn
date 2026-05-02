#pragma once

#include <filesystem>
#include <vector>
#include "tinyxml2.h"
#include "Block.h"

namespace fs = std::filesystem;

class XMLIO
{
    private:
        fs::path    filepath;
        fs::path    outputPath;
        Block       block;
    public:
        void        readXML(const fs::path& filepath);
        void        writeXML();
        void        writeXML(const Block& block);
};

