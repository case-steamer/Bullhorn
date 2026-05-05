#pragma once

#include <filesystem>
#include <vector>
#include <memory>
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
        void        initBlock();
        void        readXML(const fs::path& filepath);
        std::unique_ptr<tinyxml2::XMLDocument> buildXML();
        void        setTime(int hour, int minute);
        void        addData(const fs::path& filepath);
        //void        addData(const float volume);
        //^Will bring this function back in eventually^
        void        writeXML();
};

