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
        mTreeID=0;
        auto log = getLog();
        static ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        ImGui::Begin("Asset Browser");
        auto projDef = mProject->getProjectDefinition();

        if (projDef == nullptr)
        {
            ImGui::End();
            return;
        }

        for (auto assetPair : Constants::DREAM_ASSET_TYPES_MAP)
        {
           AssetType type = assetPair.first;
           string name = assetPair.second;

           name = Constants::getAssetTypeReadableNameFromString(name);
           if (ImGui::TreeNodeEx((void*)(intptr_t)mTreeID++,node_flags,name.c_str(),0))
           {
               auto assets = projDef->getAssetDefinitionsVector(type);
               for (auto asset : assets)
               {
                   if (ImGui::TreeNodeEx
                        (
                            (void*)(intptr_t)mTreeID++,
                            node_flags | ImGuiTreeNodeFlags_Leaf,
                            asset->getName().c_str(),
                            0
                        )
                    ){
                       if (ImGui::IsItemClicked())
                        {
                            log->error("Asset Definition Clicked {}", asset->getName());
                            mPropertiesWindowHandle->pushPropertyTarget(
                                PROP_TYPE_ASSET,
                                asset,
                                nullptr
                            );
                        }
                       ImGui::TreePop();
                   }
               }
               ImGui::TreePop();
           }
        } // Asset Type Node
        ImGui::End();
    }
}
