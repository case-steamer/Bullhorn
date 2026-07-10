#pragma once

#include "IPanel.h"
#include "Driver.h"

class NotifBar : public IPanel
{
    public:
        NotifBar(Driver& driver);

        void render() override;

    private:
        Driver& driver;
};
