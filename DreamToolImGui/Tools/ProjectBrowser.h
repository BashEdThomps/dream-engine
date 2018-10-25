#include "DTWidget.h"

namespace Dream
{
    class SceneObjectDefinition;
}

using Dream::SceneObjectDefinition;

namespace DreamTool
{
    class PropertiesWindow;
    class ProjectBrowser : public DTWidget
    {
    public:
        ProjectBrowser(Project* proj, PropertiesWindow*);
        ~ProjectBrowser() override;
        void draw() override;
    private:
        void addSceneObject(SceneObjectDefinition* def);
        PropertiesWindow* mPropertiesWindowHandle;
    };
}
