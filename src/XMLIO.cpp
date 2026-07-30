#include "XMLIO.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <stdexcept>

namespace
{
    tinyxml2::XMLElement* checkForElement(tinyxml2::XMLNode* n, const char* element)
    {
        tinyxml2::XMLElement* object = n->FirstChildElement(element);
        if (object == nullptr)
            throw std::runtime_error(std::string(element) + " Element not found!");
        return object;
    }

    const char* textGetter(tinyxml2::XMLElement* el)
    {
        auto text = el->GetText();
        if (text == nullptr)
            throw std::runtime_error(std::string(el->Name()) + " text not found!");

        return text;
    }
}

void XMLIO::initBlock()
{
    block = Block{};
}

void XMLIO::initQueue()
{
    queue = Queue{};
}

bool XMLIO::readBlock(const fs::path& filepath)
{
    bool flag = false;
    try
    {
        block.allTracks.clear();
        tinyxml2::XMLDocument doc;
        if (doc.LoadFile(filepath.c_str()) != tinyxml2::XML_SUCCESS)
        {
            auto fault = doc.ErrorStr();
            throw std::runtime_error(fault);
        }
    
        auto*   block_element   = checkForElement(&doc, "block");
        auto*   time_element    = checkForElement(block_element, "timecode");
        std::string timecode    = textGetter(time_element);
        block.hour              = std::stoi(timecode.substr(0,2));
        block.minute            = std::stoi(timecode.substr(3,2));
    
        auto* track_element     = block_element->FirstChildElement("track");
        while (track_element    != nullptr)
        {
            Block::Track track;
            track.filepath      = textGetter(checkForElement(track_element, "filepath"));
            track.volume        = std::stof(textGetter(checkForElement(track_element, "volume")));
            block.allTracks.push_back(track);
            track_element = track_element->NextSiblingElement("track");
        }
        flag = true;
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << "\n";
    }
    return flag;
}

bool XMLIO::readQueue(const fs::path& filepath)
{
    bool flag = false;
    try
    {
        tinyxml2::XMLDocument doc;
        if (doc.LoadFile(filepath.c_str()) != tinyxml2::XML_SUCCESS)
        {
            auto fault = doc.ErrorStr();
            throw std::runtime_error(fault);
        }
    
        auto*   queue_element   = checkForElement(&doc, "queue");
        auto*   block_entry     = queue_element->FirstChildElement("block");
    
        initQueue();
    
        while   (block_entry  != nullptr)
        {
            initBlock();
            auto*   path_element    = checkForElement(block_entry, "filepath");
            const char* behavior = block_entry->Attribute("behavior");
    
            Queue::BlockEntry entry;
            entry.filepath = textGetter(path_element);
            if (readBlock(entry.filepath))
            {
                entry.block         = block;
                entry.isOverride    = (behavior != nullptr && std::string(behavior) == "override");
                queue.allBlocks.push_back(entry);
            }
            block_entry = block_entry->NextSiblingElement("block");
        }
        flag = true;
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << "\n";
    }
    return flag;
}

std::unique_ptr<tinyxml2::XMLDocument> XMLIO::buildBlock()
{
    auto    doc = std::make_unique<tinyxml2::XMLDocument>();
    auto*   block_element   = doc->NewElement("block");
    auto*   time_element    = doc->NewElement("timecode");

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << block.hour << ":"
        << std::setw(2) << std::setfill('0') << block.minute;
    std::string timecode = oss.str();
    time_element->SetText(timecode.c_str());
    block_element->InsertEndChild(time_element);

    for (const Block::Track& track : block.allTracks)
    {
        auto* track_element =   doc->NewElement("track");
        auto* filepath_element =doc->NewElement("filepath");
        auto* volume_element =  doc->NewElement("volume");
        //auto* output_element =    doc->NewElement("output_filepath");
        
        filepath_element->SetText(track.filepath.c_str());
        volume_element->SetText(std::to_string(track.volume).c_str());
        //output_element    = this logic will be created later.

        track_element->InsertEndChild(volume_element);
        track_element->InsertEndChild(filepath_element);
        block_element->InsertEndChild(track_element);
    }
    doc->InsertEndChild(block_element);
    return doc;
}

std::unique_ptr<tinyxml2::XMLDocument> XMLIO::buildQueue()
{
    auto    doc             = std::make_unique<tinyxml2::XMLDocument>();
    auto*   queue_element   = doc->NewElement("queue");
    for (const Queue::BlockEntry& block : queue.allBlocks)
    {
        auto* block_entry = doc->NewElement("block");
        auto* filepath_element=doc->NewElement("filepath");

        filepath_element->SetText(block.filepath.c_str());
        block_entry->SetAttribute("behavior", block.isOverride ? "override" : "wait");

        block_entry->InsertEndChild(filepath_element);
        queue_element->InsertEndChild(block_entry);
    }
    doc->InsertEndChild(queue_element);
    return doc;
}

void XMLIO::setTime(int hour, int minute)
{
    block.hour = hour;
    block.minute = minute;
}

void XMLIO::addData(const fs::path& filepath)
{
    Block::Track track;
    track.filepath = filepath;
    block.allTracks.push_back(track);
}

void XMLIO::addBlock(const Queue::BlockEntry& entry)
{
    queue.allBlocks.push_back(entry);
}

void XMLIO::subtractBlock(const fs::path filepath)
{
    const auto it = std::find_if(queue.allBlocks.begin(), queue.allBlocks.end(), [&](Queue::BlockEntry& i){return i.filepath == filepath;});
    if (it != queue.allBlocks.end())
        queue.allBlocks.erase(it);
}

void XMLIO::writeBlock(fs::path filepath) 
{
    auto doc_to_write = buildBlock();
    doc_to_write->SaveFile(filepath.c_str());
}

void XMLIO::writeQueue(fs::path filepath)
{
    auto doc_to_write = buildQueue();
    doc_to_write->SaveFile(filepath.c_str());
}

Block& XMLIO::getBlock()
{
    return block;
}

Queue& XMLIO::getQueue()
{
    return queue;
}

Queue& XMLIO::getQueue(const fs::path& filepath)
{
    readQueue(filepath);
    return queue;
}

