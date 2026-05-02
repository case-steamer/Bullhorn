#include "XMLIO.h"

#include <iostream>

filepath = "/home/case_steamer/CPP_Projects/LearnDependencies/Bullhorn/test-assets/test.xml"

void XMLIO::readXML(const fs::path& filepath)
{
    this->filepath  = filepath;
    this->block     = //block defined in xml at filepath.
}

void XMLIO::writeXML()
{
    //transpose block as defined by user onto this->block dynamically, then write this->block to XML.
}

void XMLIO::writeXML(block&)
{
    //Whatever exists at this->block gets written to our xml
}
