#pragma once

#include "ImGuiWidget.h"

namespace Dream
{
    class SceneObjectDefinition;
    class Definition;
}

using namespace Dream;

namespace DreamTool
{
    struct SceneObjectDragSource
    {
       SceneObjectDefinition* parentDef = nullptr;
       SceneObjectDefinition* objectDef = nullptr;
    };


    class ProjectBrowser : public ImGuiWidget
    {
    public:
        ProjectBrowser(DTState* proj);
        ~ProjectBrowser() override;
        void draw() override;

    private:
        void drawAssetTree();
        void drawProjectTree();
        void addSceneObject(SceneObjectDefinition* def);
        const ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
        const ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
        vector<Definition*> mSelectedNodes;
        SceneObjectDragSource mDragDropSource;
    };
}
