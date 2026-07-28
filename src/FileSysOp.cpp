#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <cctype>

#include "Driver.h"
#include "FileSysOp.h"

FileSysOp::FileSysOp(Driver& driver) : driver(driver)
{
}

bool    FileSysOp::isValid(const fs::path& input) const
{
    return fs::exists(input);
}

bool    FileSysOp::isValid(const fs::path& input, const std::vector<std::string>& extensions) const
{
    if (!fs::exists(input)) return false;
    for (const std::string& suffix : extensions)
    {
        if (input.extension() == suffix) return true;
    }
    return false;
}

bool    FileSysOp::isValid(const fs::path& input, const Queue& queue) const
{
    if (!fs::exists(input)) return false;
    for (Queue::BlockEntry ent : queue.allBlocks)
    {
        if (input == ent.filepath) return true;
    }
    return false;
}

bool    FileSysOp::isValidBlockName(const fs::path& input, char identifier) const
{
    bool valid = true;
    std::string name = input.stem().string();
    if (name.size() != 5)
        return false;
    if (name[0] != identifier)
        return false;

    name.erase(0, 1);
    for (int i = 0; i < 4; i++)
    {
        if (!std::isdigit(static_cast<unsigned char>(name[i])))
        {
            valid = false;
        }
    }
    return valid;
}

std::optional<int>     FileSysOp::nextBlockID(const Queue& queue, char identifier) const
{
    std::vector<int> blockStems;
    std::optional<int> newID;
    //The following auto& be will be a Queue::BlockEntry
    for (const auto& be : queue.allBlocks)
    {
        if (isValidBlockName(be.filepath, identifier))
        {
            std::string beStem = be.filepath.stem().string().erase(0, 1);
            int stemNum = std::stoi(beStem);
            blockStems.push_back(stemNum);
        }
    } 
    for (int i = 0; i < 1440; i++)
    {
        if (std::find(blockStems.begin(), blockStems.end(), i) == blockStems.end())
        {
            newID = i;
            break;
        }
    }
    return newID;
}

void    FileSysOp::deleteFile(const fs::path& input)
{
    if (fs::exists(input))
        fs::remove(input);
}

std::optional<Project>    FileSysOp::createNewProject(const fs::path& root) const
{
    Project candidate(root);
    try
    {
        if (fs::create_directory(candidate.homeDir))
        {
            fs::create_directory(candidate.media);
            fs::create_directory(candidate.blocks);
            auto fStream = std::ofstream(candidate.queue);

            if (fStream.is_open())
                fStream.close();
            else
            {
                driver.pushMessage("Generation Failed.");
                driver.pushMessage("Deleting Artifacts");
     
                fs::remove_all(candidate.homeDir);
     
                driver.pushMessage("Done!");
                return std::nullopt;
            }
        }
        else
        {
            driver.pushMessage("Generation Failed.");
            driver.pushMessage("Deleting Artifacts");
    
            fs::remove_all(candidate.homeDir);
    
            driver.pushMessage("Done!");
            return std::nullopt;
        }
    }
    catch (const std::exception& e)
    {
        driver.pushMessage("Project Generation Failed with exception:\n" + std::string(e.what()));
        driver.pushMessage("Deleting Artifacts");

        fs::remove_all(candidate.homeDir);

        driver.pushMessage("Done!");
        return std::nullopt;
    }
    return candidate;
}

std::optional<Project> FileSysOp::loadProject(const fs::path& root) const
{
    bool members[3] = {false, false, false};

    Project base = Project(root);

    if (fs::exists(base.blocks))
        members[0] = true;
    if (fs::exists(base.media))
        members[1] = true;
    if (fs::exists(base.queue))
        members[2] = true;

    for (bool m : members)
    {
        if (!m)
            return std::nullopt;
    }

    return base;
}

fs::path FileSysOp::getMediaPath() const
{
    return mediaPath;
}
