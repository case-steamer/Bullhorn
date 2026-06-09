#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "Ipanel.h"
#include "Driver.h"

namespace fs = std::filesystem;

class Toolbar : public IPanel
{
    public:
        Toolbar(Driver& driver);
        void render() override;
