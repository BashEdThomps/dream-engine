#include "AssetBrowser.h"

using Dream::Project;
using Dream::Constants;
using Dream::AssetType;
using Dream::IAssetDefinition;

namespace DreamTool
{
    AssetBrowser::AssetBrowser(Project* proj)
        : DTWidget(proj)
    {
        setLogClassName("AssetBrowser");
    }

    AssetBrowser::~AssetBrowser
    ()
    {

    }

    void AssetBrowser::draw
    ()
    {
        auto log = getLog();
        ImGui::Begin("Asset Browser");
        auto projDef = mProject->getProjectDefinition();
        for (auto assetPair : Constants::DREAM_ASSET_TYPES_MAP)
        {
           AssetType type = assetPair.first;
           string name = assetPair.second;
           name = Constants::getAssetTypeReadableNameFromString(name);
           if (ImGui::TreeNode(name.c_str()))
           {
               auto assets = projDef->getAssetDefinitionsList(type);
               for (auto asset : assets)
               {
                   if (ImGui::TreeNode(asset->getName().c_str()))
                   {
                       ImGui::TreePop();
                   }
               }
               ImGui::TreePop();
           }
        } // Asset Type Node
        ImGui::End();
    }
}
