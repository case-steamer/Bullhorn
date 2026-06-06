#pragma once

#include "IPanel.h"
#include "Driver.h"

class BlockPanel : public IPanel
{
    public:
        BlockPanel(Driver& driver);

        void                        render() override;

    private:
        Driver&     driver;
};
