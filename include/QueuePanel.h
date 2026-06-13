#pragma once

#include "IPanel.h"
#include "Driver.h"

class QueuePanel : public IPanel
{
    public:
        QueuePanel(Driver& driver);

        void                        render() override;
        void                        displayQueue();
        int                         getSelected() const;
        void                        clearSelection();

    private:
        Driver&     driver;
        int         selIndex = -1;
};

