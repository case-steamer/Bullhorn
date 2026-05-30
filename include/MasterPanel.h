#include "IPanel.h"
#include "Driver.h"

class MasterPanel : public IPanel
{
    public:
        MasterPanel(Driver& driver);
        void render() override;

    private:
        Driver&     driver;
        //```IPanel&   subpanels``` to be added later
};
