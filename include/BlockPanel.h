#pragma once

#include "IPanel.h"
#include "Driver.h"

class BlockPanel : public IPanel
{
    public:
        BlockPanel(Driver& driver);

        void                        render() override;
        void                        displayBlock();
        int                         getSelected() const;
        void                        clearSelection();

    private:
        Driver&     driver;
        int         selIndex = -1;
};

