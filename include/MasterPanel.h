#include "IPanel.h"
#include "Driver.h"
#include "Toolbar.h"
#include "FileTreePanel.h"
#include "BlockPanel.h"
#include "QueuePanel.h"

class MasterPanel : public IPanel
{
    public:
        MasterPanel(Driver& driver);
        void render() override;

    private:
        Driver&         driver;
        Toolbar         toolbar;
        FileTreePanel   filePanel;
        BlockPanel      blockPanel;
        QueuePanel      queuePanel;
        //```IPanel&   subpanels``` to be added later
};
