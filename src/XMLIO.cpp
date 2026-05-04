#include "XMLIO.h"

#include <iostream>

filepath = "/home/case_steamer/CPP_Projects/LearnDependencies/Bullhorn/test-assets/test.xml"

void XMLIO::initBlock()
{
    block = Block{};
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
