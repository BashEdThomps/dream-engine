#pragma once

#include "ImGuiWidget.h"

namespace Dream
{
    class SceneObjectDefinition;
}

using Dream::SceneObjectDefinition;

namespace DreamTool
{
    class ProjectBrowser : public ImGuiWidget
    {
    public:
        ProjectBrowser(DTState* proj);
        ~ProjectBrowser() override;
        void draw() override;

    private:
        void drawAssetTree();
        void drawProjectTree();
        void addSceneObject(int treeId, SceneObjectDefinition* def);
        const ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
        const ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;

    };
}
