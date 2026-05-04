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

    auto*   time_element    = doc.FirstChildElement("timecode").GetText();
    block.hour              = time_element[0] + time_element[1];
    block.minute            = time_element[3] + time_element[4];

    Block::Track track;
    auto*   track_element   = doc.FirstChildElement("track");
    auto*   filepath_element= track_element.FirstChildElement("filepath").GetText();
    auto*   volume_element= track_element.FirstChildElement("volume").GetText();
    track.filepath          = filepath_element;
    track.volume            = volume_element;
    track.outputPath        = filepath_element;
    block.allTracks.push_back(track);
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
