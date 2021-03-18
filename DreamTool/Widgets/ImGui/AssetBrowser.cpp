#include "AssetBrowser.h"

#ifdef WIN32
#include <Windows.h>
#endif

#include "PropertiesWindow.h"
#include "DreamToolContext.h"

#include <DreamCore.h>
#include <nfd.h>
#include <sstream>

using std::stringstream;

namespace octronic::dream::tool
{
    AssetBrowser::AssetBrowser(DreamToolContext* project)
        : ImGuiWidget(project)
    {}

    AssetBrowser::~AssetBrowser
    ()
    {
    }

    void
    AssetBrowser::draw
    ()
    {
        auto project = mContext->getProject();
        if (project)
        {
            ImGui::Begin("Assets",&mVisible);
            auto projDef = mContext->getProject()->getDefinition();

            if (projDef == nullptr)
            {
                return;
            }

            ImGui::PushID("AssetTree");
            for (auto name : Constants::DREAM_ASSET_TYPES_READABLE_VECTOR)
            {
                AssetType type = Constants::getAssetTypeEnumFromString(name);
                auto assets = projDef->getAssetDefinitionsVector(type);
                auto typeGroups = projDef->getAssetDefinitionGroups()[type];
                stringstream nameCount;
                nameCount <<  name << " (" <<  assets.size() << ")";
                ImGui::PushID(name.c_str());
                static string selectedAssetType = "";
                ImGui::SetNextTreeNodeOpen(selectedAssetType.compare(name) == 0);
                bool headerOpen = ImGui::CollapsingHeader(nameCount.str().c_str(),node_flags);

                // Context Menu
                if (ImGui::BeginPopupContextItem())
                {
                    char buf[buf_sz];
                    snprintf(buf,buf_sz,"New %s",name.c_str());
                    bool newClicked = ImGui::MenuItem(buf);
                    if (newClicked)
                    {
                        auto newDef = projDef->createNewAssetDefinition(type);
                        mContext->getPropertiesWindow()->pushPropertyTarget(PropertyType_Asset,newDef,nullptr);
                        projDef->regroupAssetDefinitions();
                    }
                    ImGui::EndPopup();
                }

                if (headerOpen)
                {
                    // Group Nodes
                    selectedAssetType = name;
                    int assetDefTreeId = 0;
                    for (string group : typeGroups)
                    {
                        ImGui::PushID(group.c_str());
                        if (ImGui::TreeNode(group.c_str()))
                        {
                            // Asset Nodes
                            for (auto asset : assets)
                            {
                                if (asset->getGroup().compare(group) == 0)
                                {
                                    bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)++assetDefTreeId,leaf_flags,asset->getName().c_str(),0);

                                    // Asset Def Contex Menu
                                    if (ImGui::BeginPopupContextItem())
                                    {
                                        char buf[buf_sz];
                                        snprintf(buf,buf_sz,"Delete %s",asset->getName().c_str());
                                        bool deleteClicked = ImGui::MenuItem(buf);
                                        if (deleteClicked)
                                        {
                                        }
                                        ImGui::EndPopup();
                                    }

                                    if (nodeOpen)
                                    {
                                        if (ImGui::IsItemClicked())
                                        {
                                            LOG_DEBUG("AssetBrowser: Asset Definition Clicked {}", asset->getName());
                                            mContext->getPropertiesWindow()->pushPropertyTarget(PropertyType_Asset, asset, nullptr);
                                        }
                                        ImGui::TreePop();
                                    }
                                }
                            }
                            ImGui::TreePop();
                        }
                        ImGui::PopID();
                    }
                }
                ImGui::PopID();
            } // Asset Type Node
            ImGui::PopID();

            ImGui::End();
        }
    }
}

