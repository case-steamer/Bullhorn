#include <fstream>

#include "Driver.h"
#include "FileSysOp.h"

FileSysOp::FileSysOp(Driver& driver) : driver(driver)
{
}

bool    FileSysOp::isValid(const fs::path& input)
{
    return fs::exists(input);
}

bool    FileSysOp::isValid(const fs::path& input, const std::vector<std::string>& extensions)
{
    if (!fs::exists(input)) return false;
    for (const std::string& suffix : extensions)
    {
        if (input.extension() == suffix) return true;
    }
    return false;
}

bool    FileSysOp::isValid(const fs::path& input, const Queue& queue)
{
    if (!fs::exists(input)) return false;
    for (Queue::BlockEntry ent : queue.allBlocks)
    {
        if (input == ent.filepath) return true;
    }
    return false;
}

void    FileSysOp::deleteFile(fs::path& input)
{
    fs::remove(input);
}

void    FileSysOp::createNewProject()
{
    if (fs::create_directory(driver.activeProjectFile))
    {
        driver.pushMessage("Creating Project...");
        std::string media = driver.activeProjectFile/std::string("media");
        fs::path mediaPath = media;
        if (fs::create_directory(mediaPath))
        {
            driver.pushMessage("Creating Project...");
        }
        else
        {
            driver.pushMessage("Project Generation failed");
            return;
        }
        std::string blocks = driver.activeProjectFile/std::string("blocks");
        fs::path blocksPath = blocks;
        if (fs::create_directory(blocksPath))
        {
            driver.pushMessage("Creating Project...");
        }
        else
        {
            driver.pushMessage("Project Generation failed");
            return;
        }
        std::string nuQueue = driver.activeProjectFile/std::string("QUEUE.xml");
        std::ofstream outFile(nuQueue);
        if (outFile.is_open())
        {
            outFile.close();
        }
        driver.pushMessage("Creation Success!");
    }
    else
    {
        driver.pushMessage("Failed. Could not create Project.");
    }
}

fs::path FileSysOp::getMediaPath() const
{
    return mediaPath;
}
