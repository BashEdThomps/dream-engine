#include "DefinitionBrowser.h"

#include "Widgets/ImGui/PropertiesWindow/PropertiesWindow.h"
#include "DreamToolContext.h"

#include <DreamCore.h>
#include <nfd.h>
#include <sstream>

using std::stringstream;

namespace octronic::dream::tool
{
  DefinitionBrowser::DefinitionBrowser
  (DreamToolContext& project)
    : ImGuiWidget(project,true)
  {}

  void
  DefinitionBrowser::draw
  ()
  {
    auto& pCtxOpt = getContext().getProjectContext();

    ImGui::Begin("Definitions",&mVisible);
    if (pCtxOpt)
    {
      auto& pCtx = pCtxOpt.value();

      auto& projDefOpt = pCtx.getProjectDefinition();

      if (!projDefOpt.has_value())
      {
        return;
      }

      auto& projDef = projDefOpt.value();

      // Template Entities =====================================================
      {
        string name = "Template Entities";
        auto templateEntities = projDef.getTemplateEntityDefinitionsVector();
        stringstream nameCount;
        nameCount << name << " (" <<  templateEntities.size() << ")";
        ImGui::PushID(name.c_str());

        ImGui::SetNextTreeNodeOpen(mOpenHeaders[name]);
        bool nodeResult = ImGui::CollapsingHeader(nameCount.str().c_str(),node_flags);
        mOpenHeaders[name] = nodeResult;

        if (mOpenHeaders[name])
        {
          if (ImGui::Button("+"))
          {
            auto& newDef = projDef.createTemplateEntityDefinition();
            getContext().getPropertiesWindow().pushPropertyTarget(PropertyType_TemplateEntity,newDef);
          }

          // Group Nodes
          int assetDefTreeId = 0;
          // Asset Nodes
          for (auto& templateEntityWrap : templateEntities)
          {
						auto& templateEntity = templateEntityWrap.get();
            bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)++assetDefTreeId,leaf_flags,templateEntity.getName().c_str(),0);

            // Asset Def Contex Menu
            if (ImGui::BeginPopupContextItem())
            {
              char buf[buf_sz];
              snprintf(buf,buf_sz,"Delete %s",templateEntity.getName().c_str());
              bool deleteClicked = ImGui::MenuItem(buf);
              if (deleteClicked)
              {
                projDef.removeTemplateEntityDefinitionByUuid(templateEntity.getUuid());
              }
              ImGui::EndPopup();
            }

            if (nodeOpen)
            {
              if (ImGui::IsItemClicked())
              {
                LOG_DEBUG("AssetBrowser: Template Entity Definition Clicked {}", templateEntity.getName());
                getContext().getPropertiesWindow().pushPropertyTarget(PropertyType_TemplateEntity, templateEntity);
              }
              ImGui::TreePop();
            }
          }
        }
        ImGui::PopID();
      }

      ImGui::Separator();

      // Assets ================================================================

      ImGui::PushID("AssetTree");
      for (auto& readablePair : AssetTypeHelper::ReadableTypesMap)
      {
        AssetType type = readablePair.first;
        string name = readablePair.second;

        auto assets = projDef.getAssetDefinitionsVector(type);
        stringstream nameCount;
        nameCount <<  name << " (" <<  assets.size() << ")";
        ImGui::PushID(name.c_str());
        ImGui::SetNextTreeNodeOpen(mOpenHeaders[name]);
        bool nodeResult = ImGui::CollapsingHeader(nameCount.str().c_str(),node_flags);
        mOpenHeaders[name] = nodeResult;

        if (mOpenHeaders[name])
        {
          if (ImGui::Button("+"))
          {
            auto& newDef = projDef.createAssetDefinition(type);
            getContext().getPropertiesWindow().pushPropertyTarget(PropertyType_Asset,newDef);
          }

          // Group Nodes
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
                projDef.removeAssetDefinitionByUuid(asset.getAssetType(), asset.getUuid());
              }
              ImGui::EndPopup();
            }

            if (nodeOpen)
            {
              if (ImGui::IsItemClicked())
              {
                LOG_DEBUG("AssetBrowser: Asset Definition Clicked {}", asset.getName());
                getContext().getPropertiesWindow().pushPropertyTarget(PropertyType_Asset, asset);
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

