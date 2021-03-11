#pragma once

#include "ImGuiWidget.h"

namespace octronic::dream
{
    class EntityDefinition;
    class Definition;
}

#define buf_sz 128

namespace octronic::dream::tool
{
    struct EntityDragSource
    {
       EntityDefinition* parentDef = nullptr;
       EntityDefinition* objectDef = nullptr;
    };


    class ProjectBrowser : public ImGuiWidget
    {
    public:
        ProjectBrowser(DreamToolContext* proj);
        ~ProjectBrowser() override;
        void draw() override;

    private:
        void addEntity(EntityDefinition* def);
        const ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
        const ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
        vector<Definition*> mSelectedNodes;
        EntityDragSource mDragDropSource;
    };
}
