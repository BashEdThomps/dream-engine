#include "AssetBrowser.h"
#include "PropertiesWindow.h"

using Dream::Project;
using Dream::Constants;
using Dream::AssetType;
using Dream::IAssetDefinition;

namespace DreamTool
{
    AssetBrowser::AssetBrowser(Project* proj, PropertiesWindow* properties)
        : DTWidget(proj),
          mPropertiesWindowHandle(properties)
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
        static ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
        ImGui::Begin("Asset Browser");
        auto projDef = mProject->getProjectDefinition();
        /*
        ImGui::Columns(Constants::DREAM_ASSET_TYPES_MAP.size());
        for (auto assetPair : Constants::DREAM_ASSET_TYPES_MAP)
        {
             AssetType type = assetPair.first;
             string name = assetPair.second;
             name = Constants::getAssetTypeReadableNameFromString(name);
             ImGui::Text(name.c_str());
             ImGui::NextColumn();
        }
        */

        for (auto assetPair : Constants::DREAM_ASSET_TYPES_MAP)
        {
           AssetType type = assetPair.first;
           string name = assetPair.second;
           name = Constants::getAssetTypeReadableNameFromString(name);
           if (ImGui::TreeNodeEx((void*)(intptr_t)0,node_flags,name.c_str(),0))
           {
               auto assets = projDef->getAssetDefinitionsVector(type);
               for (auto asset : assets)
               {
                   if (ImGui::TreeNodeEx((void*)(intptr_t)0,node_flags,asset->getName().c_str(),0))
                   {
                       if (ImGui::IsItemClicked())
                        {
                            log->error("Asset Definition Clicked {}", asset->getName());
                            mPropertiesWindowHandle->setPropertyType(PROP_TYPE_ASSET);
                            mPropertiesWindowHandle->setDefinition(asset);
                            mPropertiesWindowHandle->setRuntime(nullptr);
                        }
                       ImGui::TreePop();
                   }
               }
               ImGui::TreePop();
               //ImGui::NextColumn();
           }
        } // Asset Type Node
        ImGui::End();
    }
}
