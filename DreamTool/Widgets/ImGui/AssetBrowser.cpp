#include "AssetBrowser.h"

#include "Widgets/ImGui/PropertiesWindow/PropertiesWindow.h"
#include "DreamToolContext.h"

#include <DreamCore.h>
#include <nfd.h>
#include <sstream>

using std::stringstream;

namespace octronic::dream::tool
{
  AssetBrowser::AssetBrowser(DreamToolContext& project)
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
    auto& pCtxOpt = getContext().getProjectContext();

    ImGui::Begin("Assets",&mVisible);
    if (pCtxOpt)
    {
      auto& pCtx = pCtxOpt.value();

      auto& projDefOpt = pCtx.getProjectDefinition();

      if (!projDefOpt.has_value())
      {
        return;
      }

      auto& projDef = projDefOpt.value();

      ImGui::PushID("AssetTree");
      for (auto readablePair : AssetTypeHelper::ReadableTypesMap)
      {
        AssetType type = readablePair.first;
        string name = readablePair.second;

        auto assets = projDef.getAssetDefinitionsVector(type);
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
            auto& newDef = projDef.createNewAssetDefinition(type);
            getContext().getPropertiesWindow().pushPropertyTarget({PropertyType_Asset,newDef});
          }
          ImGui::EndPopup();
        }

        if (headerOpen)
        {
          // Group Nodes
          selectedAssetType = name;
          int assetDefTreeId = 0;
          // Asset Nodes
          for (auto& assetWrap : assets)
          {
            auto& asset = assetWrap.get();

            bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)++assetDefTreeId,leaf_flags,asset.getName().c_str(),0);

            // Asset Def Contex Menu
            if (ImGui::BeginPopupContextItem())
            {
              char buf[buf_sz];
              snprintf(buf,buf_sz,"Delete %s",asset.getName().c_str());
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
                LOG_DEBUG("AssetBrowser: Asset Definition Clicked {}", asset.getName());
                getContext().getPropertiesWindow().pushPropertyTarget({PropertyType_Asset, asset});
              }
              ImGui::TreePop();
            }
          }
        }
        ImGui::PopID();
      } // Asset Type Node
      ImGui::PopID();
    }
    ImGui::End();
  }
}

