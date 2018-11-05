#include "ImGuiWidget.h"

namespace Dream
{
    class SceneObjectDefinition;
}

using Dream::SceneObjectDefinition;

namespace DreamTool
{
    class PropertiesWindow;
    class ProjectBrowser : public ImGuiWidget
    {
    public:
        ProjectBrowser(Project* proj, PropertiesWindow*);
        ~ProjectBrowser() override;
        void draw() override;
    private:
        void drawAssetTree();
        void drawProjectTree();
        void addSceneObject(int treeId, SceneObjectDefinition* def);
        PropertiesWindow* mPropertiesWindowHandle;
        const ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
        const ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;

    };
}
