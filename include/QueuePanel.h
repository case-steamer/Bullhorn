#pragma once

#include "IPanel.h"
#include "Driver.h"

class QueuePanel : public IPanel
{
    public:
        QueuePanel(Driver& driver);

        void                        render() override;

    private:
        Driver&     driver;
};

