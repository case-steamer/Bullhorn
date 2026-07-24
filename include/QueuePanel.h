#pragma once

#include <filesystem>

#include "IPanel.h"
#include "Driver.h"

namespace fs = std::filesystem;

class QueuePanel : public IPanel
{
    public:
        QueuePanel(Driver& driver);

        void                        render() override;
        void                        displayQueue();
        int                         getSelected() const;
        void                        refreshBuffers();
        void                        clearSelection();

    private:
        Driver&                     driver;
        fs::path                    currentPath;
        std::vector<std::array<char, 6>>            timecodeBuffers;
        std::vector<bool>                           timecodeFlags;
        int                         selIndex = -1;
        bool                        pendingSort = false;
        void                        sortBlocks();
};

