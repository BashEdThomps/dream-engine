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
    class AssetBrowser : public ImGuiWidget
    {
    public:
        AssetBrowser(DreamToolContext* proj);
        ~AssetBrowser() override;
        void draw() override;

    private:
        const ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
        const ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
        vector<Definition*> mSelectedNodes;
    };
}
