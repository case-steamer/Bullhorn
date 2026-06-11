#include "XMLIO.h"

#include <iostream>
#include <iomanip>
#include <sstream>

void XMLIO::initBlock()
{
    block = Block{};
}

void XMLIO::initQueue()
{
    queue = Queue{};
}

void XMLIO::readBlock(const fs::path& filepath)
{
    block.allTracks.clear();
    tinyxml2::XMLDocument doc;
    doc.LoadFile(filepath.c_str());

    auto*   block_element   = doc.FirstChildElement("block");
    auto*   time_element    = block_element->FirstChildElement("timecode");
    std::string timecode    = time_element->GetText();
    block.hour              = std::stoi(timecode.substr(0,2));
    block.minute            = std::stoi(timecode.substr(3,2));

    auto* track_element     = block_element->FirstChildElement("track");
    while (track_element    != nullptr)
    {
        Block::Track track;
        track.filepath      = track_element->FirstChildElement("filepath")->GetText();
        track.volume        = std::stof(track_element->FirstChildElement("volume")->GetText());
        block.allTracks.push_back(track);
        track_element = track_element->NextSiblingElement("track");
    }
}

void XMLIO::readQueue(const fs::path& filepath)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile(filepath.c_str());

    auto*   queue_element   = doc.FirstChildElement("queue");
    auto*   block_entry     = queue_element->FirstChildElement("block");

    while   (block_entry  != nullptr)
    {
        initBlock();
        auto*   path_element    = block_entry->FirstChildElement("filepath");
        readBlock(fs::path(path_element->GetText()));
        const char* behavior = block_entry->Attribute("behavior");

        Queue::BlockEntry entry;
        entry.filepath  = path_element->GetText();
        entry.block     = block;
        entry.isOverride  = (std::string(behavior) == "override");
        queue.allBlocks.push_back(entry);
        block_entry             = block_entry->NextSiblingElement("block");
    }
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

void XMLIO::writeBlock(fs::path filepath) 
{
    auto doc_to_write = buildBlock();
    doc_to_write->SaveFile(filepath.c_str());
}

void XMLIO::writeQueue()
{
    auto doc_to_write = buildQueue();
    doc_to_write->SaveFile("/home/case_steamer/CPP_Projects/LearnDependencies/Bullhorn/test-assets/test_queue.xml");
}

Block& XMLIO::getBlock()
{
    return block;
}

Block& XMLIO::getBlock(const fs::path& filepath)
{
    readBlock(filepath);
    return block;
}

const Queue& XMLIO::getQueue()
{
    return queue;
}

