#pragma once

#include "ImGuiWidget.h"

namespace Dream
{
    class EntityDefinition;
    class Definition;
}

#define buf_sz 128

using namespace Dream;

namespace DreamTool
{
    struct EntityDragSource
    {
       EntityDefinition* parentDef = nullptr;
       EntityDefinition* objectDef = nullptr;
    };


    class ProjectBrowser : public ImGuiWidget
    {
    public:
        ProjectBrowser(DTContext* proj);
        ~ProjectBrowser() override;
        void draw() override;

    private:
        void drawAssetTree();
        void drawProjectTree();
        void addEntity(EntityDefinition* def);
        const ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
        const ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
        vector<Definition*> mSelectedNodes;
        EntityDragSource mDragDropSource;
    };
}
