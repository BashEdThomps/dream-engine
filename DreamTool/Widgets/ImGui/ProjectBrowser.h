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
       shared_ptr<EntityDefinition> parentDef;
       shared_ptr<EntityDefinition> objectDef;
    };


    class ProjectBrowser : public ImGuiWidget
    {
    public:
        ProjectBrowser(DreamToolContext* proj);
        ~ProjectBrowser() override;
        void draw() override;

    private:
        void addEntity(const shared_ptr<EntityDefinition>& def);
        const ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
        const ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
        vector<shared_ptr<Definition>> mSelectedNodes;
        EntityDragSource mDragDropSource;
    };
}
