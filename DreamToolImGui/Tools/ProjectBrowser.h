#include "DTWidget.h"

namespace Dream
{
    class SceneObjectDefinition;
}

using Dream::SceneObjectDefinition;

namespace DreamTool
{
    class ProjectBrowser : public DTWidget
    {
    public:
        ProjectBrowser(Project* proj);
        ~ProjectBrowser() override;
        void draw() override;
    private:
        void addSceneObject(SceneObjectDefinition* def);
    };
}
