#include "ImGuiWidget.h"

namespace Dream
{
    class SceneObjectDefinition;
}

using Dream::SceneObjectDefinition;

namespace DreamTool
{
    class PropertiesWindow;
    class SelectionHighlighterWidget;
    class ProjectBrowser : public ImGuiWidget
    {
    public:
        ProjectBrowser(Dream::Project* proj, PropertiesWindow*);
        ~ProjectBrowser() override;
        void draw() override;
        void setSelectionHighlighterWidget( SelectionHighlighterWidget* shw);

    private:
        void drawAssetTree();
        void drawProjectTree();
        void addSceneObject(int treeId, SceneObjectDefinition* def);
        PropertiesWindow* mPropertiesWindowHandle;
        SelectionHighlighterWidget* mSelectionHighlighter;
        const ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
        const ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;

    };
}
