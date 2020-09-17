#pragma once

#include "ImGuiWidget.h"

namespace Dream
{
    class ActorDefinition;
    class Definition;
}

#define buf_sz 128

using namespace Dream;

namespace DreamTool
{
    struct ActorDragSource
    {
       ActorDefinition* parentDef = nullptr;
       ActorDefinition* objectDef = nullptr;
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
        void addActor(ActorDefinition* def);
        const ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
        const ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
        vector<Definition*> mSelectedNodes;
        ActorDragSource mDragDropSource;
    };
}
