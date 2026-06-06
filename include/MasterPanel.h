#include "IPanel.h"
#include "Driver.h"
#include "FileTreePanel.h"
#include "BlockPanel.h"

class MasterPanel : public IPanel
{
    public:
        MasterPanel(Driver& driver);
        void render() override;

    private:
        Driver&         driver;
        FileTreePanel   filePanel;
        BlockPanel      blockPanel;
        //```IPanel&   subpanels``` to be added later
};
