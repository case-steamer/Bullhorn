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
        enum Statii {GOOD, PARTIAL, FAILED};
        std::vector<fs::path> failures;
        std::vector<std::string> blockFailures;
        std::vector<std::string> failCodes;
        void        initBlock();
        void        initQueue();
        bool        readBlock(const fs::path& filepath);
        Statii        readQueue(const fs::path& filepath);
        std::unique_ptr<tinyxml2::XMLDocument> buildBlock();
        std::unique_ptr<tinyxml2::XMLDocument> buildQueue();
        void        setTime(int hour, int minute);
        void        addData(const fs::path& filepath);
        void        addBlock(const Queue::BlockEntry& entry);
        void        subtractBlock(const fs::path filepath);
        //void        addData(const float volume);
        //^Will bring this function back in eventually^
        void        writeBlock(fs::path filepath);
        void        writeQueue(fs::path filepath);
        Block& getBlock();
        Queue& getQueue();
        Queue& getQueue(const fs::path& filepath);
};

