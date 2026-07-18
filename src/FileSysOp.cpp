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

void    FileSysOp::createNewProject() const
{

    std::array<fs::path, 3> creationDirs = {
        driver.activeProjectFile,
        driver.activeProjectFile/std::string("media"),
        driver.activeProjectFile/std::string("blocks")
    };

    for (const fs::path &p : creationDirs)
    {
        if (!fs::create_directory(p))
        {
            driver.pushMessage("Project Generation Failed");
            return;
        }
        else
        {
            driver.pushMessage("Creating Project...");
        }
    }

    std::string nuQueue = driver.activeProjectFile/std::string("QUEUE.xml");
    std::ofstream outFile(nuQueue);
    if (outFile.is_open())
    {
        outFile.close();
    }
    else
    {
        driver.pushMessage("Failed. Could not create Project.");
        return;
    }
    driver.pushMessage("Creation Success!");
}

fs::path FileSysOp::getMediaPath() const
{
    return mediaPath;
}
