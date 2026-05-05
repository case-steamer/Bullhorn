#include "XMLIO.h"

#include <iostream>

void XMLIO::initBlock()
{
    block = Block{};
}

void XMLIO::readXML(const fs::path& filepath)
{
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

std::unique_ptr<tinyxml2::XMLDocument> XMLIO::buildXML()
{
    auto    doc = std::make_unique<tinyxml2::XMLDocument>();
    auto*   block_element   = doc->NewElement("block");
    auto*   time_element    = doc->NewElement("timecode");

    std::string timecode    = std::to_string(block.hour) + ":" + std::to_string(block.minute);
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
