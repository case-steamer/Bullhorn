#include "IPanel.h"
#include "Driver.h"
#include "FileTreePanel.h"

class MasterPanel : public IPanel
{
    public:
        MasterPanel(Driver& driver);
        void render() override;

    private:
        Driver&         driver;
        FileTreePanel   filePanel;
        //```IPanel&   subpanels``` to be added later
};
