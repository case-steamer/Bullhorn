#pragma once

#include <filesystem>
#include <vector>
#include <memory>
#include "tinyxml2.h"
#include "Block.h"
#include "Queue.h"

namespace fs = std::filesystem;

class XMLIO
{
    private:
        fs::path    filepath;
        fs::path    outputPath;
        Block       block;
        Queue       queue;
    public:
        void        initBlock();
        void        initQueue();
        void        readBlock(const fs::path& filepath);
        void        readQueue(const fs::path& filepath);
        std::unique_ptr<tinyxml2::XMLDocument> buildBlock();
        std::unique_ptr<tinyxml2::XMLDocument> buildQueue();
        void        setTime(int hour, int minute);
        void        addData(const fs::path& filepath);
        //void        addData(const float volume);
        //^Will bring this function back in eventually^
        void        writeBlock();
        void        writeQueue();
};

