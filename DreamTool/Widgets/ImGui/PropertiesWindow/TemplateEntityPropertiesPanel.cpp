#include "TemplateEntityPropertiesPanel.h"

#include "PropertiesWindow.h"
#include "DreamToolContext.h"

namespace octronic::dream::tool
{
  TemplateEntityPropertiesPanel::TemplateEntityPropertiesPanel
  (ImGuiWidget& parent)
    : ImGuiPanel(parent),
      mNameAndUuidPanel(parent)
  {

  }

  void
  TemplateEntityPropertiesPanel::draw
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());

    if (!parent.getCurrentPropertyTarget().has_value()) return;

    auto target = parent.getCurrentPropertyTarget().value().get();

    auto entDefOpt = target.getDefinition();

    auto& dtCtx = parent.getContext();
    auto& pCtx = dtCtx.getProjectContext().value();
    auto& pDef = pCtx.getProjectDefinition().value();

    // Valid Definition
    if (!entDefOpt)
    {
      return;
    }

    auto& entDef = static_cast<TemplateEntityDefinition&>(entDefOpt.value().get());

    if (ImGui::Button("Delete"))
    {
      dtCtx.getProjectBrowser().removeNodeSelection(entDef);
      parent.removeFromHistory(entDef);
      pDef.removeTemplateEntityDefinition(entDef);
      return;
    }

    ImGui::SameLine();

    if (ImGui::Button("Duplicate"))
    {
      auto& dupe = entDef.duplicateDefinition();
      parent.pushPropertyTarget(PropertyType_TemplateEntity, dupe);
    }

    mNameAndUuidPanel.draw();

    ImGui::Separator();

    drawAssetSelection();
  }

  void
  TemplateEntityPropertiesPanel::drawAssetSelection
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    auto& dtCtx = parent.getContext();
    auto& pCtx = dtCtx.getProjectContext().value();
    auto& pDef = pCtx.getProjectDefinition().value();

    if (!parent.getCurrentPropertyTarget().has_value()) return;

    auto target = parent.getCurrentPropertyTarget().value().get();

    if (target.getType() != PropertyType_TemplateEntity) return;

    auto entDefOpt = target.getDefinition();

    if (entDefOpt)
    {
      auto& entDef = static_cast<TemplateEntityDefinition&>(entDefOpt.value().get());

      ImGui::Text("Assets");

      // Animation =======================================================
      {
        int selectedAnimationAsset = entDef.getSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_ANIMATION);
        vector<string> animationAssets = pDef.getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_ANIMATION);
        if(ImGui::Button("-##Animation"))
        {
          entDef.setAssetDefinition(AssetType::ASSET_TYPE_ENUM_ANIMATION,Uuid::INVALID);
        }
        ImGui::SameLine();
        if(ImGui::Button(">##Animation"))
        {
          if (selectedAnimationAsset < 0) return;
          auto& asset = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_ANIMATION,selectedAnimationAsset).value().get();
          parent.pushPropertyTarget(PropertyType_Asset,asset);
          return;
        }

        ImGui::SameLine();

        if(ImGuiWidget::StringCombo("Animation",&selectedAnimationAsset,animationAssets))
        {
          entDef.setSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_ANIMATION, selectedAnimationAsset);
        }
      }

      // Audio ===========================================================
      {
        int selectedAudioAsset = entDef.getSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_AUDIO);
        vector<string> audioAssets = pDef.getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_AUDIO);
        if(ImGui::Button("-##Audio"))
        {
          entDef.setAssetDefinition(AssetType::ASSET_TYPE_ENUM_AUDIO,Uuid::INVALID);
        }
        ImGui::SameLine();
        if(ImGui::Button(">##Audio"))
        {
          auto& asset = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_AUDIO,selectedAudioAsset).value().get();
          parent.pushPropertyTarget(PropertyType_Asset,asset);
          return;
        }

        ImGui::SameLine();

        if(ImGuiWidget::StringCombo("Audio",&selectedAudioAsset,audioAssets))
        {
          entDef.setSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_AUDIO, selectedAudioAsset);
        }
      }

      // Font ============================================================
      {
        int selectedFontAsset = entDef.getSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_FONT);
        vector<string> fontAssets = pDef.getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_FONT);
        if(ImGui::Button("-##Font"))
        {
          entDef.setAssetDefinition(AssetType::ASSET_TYPE_ENUM_FONT,Uuid::INVALID);
        }
        ImGui::SameLine();
        if(ImGui::Button(">##Font"))
        {
          if (selectedFontAsset < 0) return;
          auto& asset = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_FONT,selectedFontAsset).value().get();
          parent.pushPropertyTarget(PropertyType_Asset,asset);
          return;
        }

        ImGui::SameLine();

        if(ImGuiWidget::StringCombo("Font",&selectedFontAsset,fontAssets))
        {
          entDef.setSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_FONT, selectedFontAsset);
        }
      }

      // Model ===========================================================
      {
        int selectedModelAsset = entDef.getSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_MODEL);
        vector<string> modelAssets = pDef.getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_MODEL);

        if(ImGui::Button("-##Model"))
        {
          entDef.setAssetDefinition(AssetType::ASSET_TYPE_ENUM_MODEL,Uuid::INVALID);
        }
        ImGui::SameLine();

        if(ImGui::Button(">##Model"))
        {
          if (selectedModelAsset < 0) return;
          auto& asset = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_MODEL,selectedModelAsset).value().get();
          parent.pushPropertyTarget(PropertyType_Asset,asset);
          return;
        }

        ImGui::SameLine();

        if(ImGuiWidget::StringCombo("Model",&selectedModelAsset,modelAssets))
        {
          entDef.setSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_MODEL, selectedModelAsset);
        }
      }

      // Path ============================================================
      {
        int selectedPathAsset = entDef.getSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_PATH);
        vector<string> pathAssets = pDef.getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_PATH);
        if(ImGui::Button("-##Path"))
        {
          entDef.setAssetDefinition(AssetType::ASSET_TYPE_ENUM_PATH,Uuid::INVALID);
        }
        ImGui::SameLine();

        if(ImGui::Button(">##Path"))
        {
          if (selectedPathAsset < 0) return;
          auto& asset = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_PATH,selectedPathAsset).value().get();
          parent.pushPropertyTarget(PropertyType_Asset, asset);
          return;
        }

        ImGui::SameLine();

        if(ImGuiWidget::StringCombo("Path",&selectedPathAsset,pathAssets))
        {
          entDef.setSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_PATH, selectedPathAsset);
        }
      }

      // Physics Object ==================================================
      {
        int selectedPhysicsObjectAsset = entDef.getSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_PHYSICS);
        vector<string> poAssets = pDef.getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_PHYSICS);
        if(ImGui::Button("-##PhysicsObject"))
        {
          entDef.setAssetDefinition(AssetType::ASSET_TYPE_ENUM_PHYSICS,Uuid::INVALID);
        }
        ImGui::SameLine();

        if(ImGui::Button(">##PhysicsObject"))
        {
          if (selectedPhysicsObjectAsset < 0) return;
          auto& asset = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_PHYSICS,selectedPhysicsObjectAsset).value().get();
          parent.pushPropertyTarget(PropertyType_Asset, asset);
          return;
        }

        ImGui::SameLine();

        if(ImGuiWidget::StringCombo("Physics Object",&selectedPhysicsObjectAsset,poAssets))
        {
          entDef.setSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_PHYSICS,selectedPhysicsObjectAsset);
        }
      }

      // Script ==========================================================
      {
        int selectedScriptAsset = entDef.getSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT);
        vector<string> scriptAssets = pDef.getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_SCRIPT);
        if(ImGui::Button("-##Script"))
        {
          entDef.setAssetDefinition(AssetType::ASSET_TYPE_ENUM_SCRIPT,Uuid::INVALID);
        }
        ImGui::SameLine();

        if(ImGui::Button(">##Script"))
        {
          if (selectedScriptAsset < 0) return;
          auto& asset = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT,selectedScriptAsset).value().get();
          parent.pushPropertyTarget(PropertyType_Asset,asset);
          return;
        }

        ImGui::SameLine();

        if(ImGuiWidget::StringCombo("Script",&selectedScriptAsset,scriptAssets))
        {
          entDef.setSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT, selectedScriptAsset);
        }
      }

      // Texture (Sprite) ================================================
      {
        int selectedTextureAsset = entDef.getSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE);
        vector<string> textureAssets = pDef.getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_TEXTURE);

        if(ImGui::Button("-##Texture"))
        {
          entDef.setAssetDefinition(AssetType::ASSET_TYPE_ENUM_SCRIPT,Uuid::INVALID);
        }

        ImGui::SameLine();

        if(ImGui::Button(">##Texture"))
        {
          if (selectedTextureAsset < 0) return;
          auto& asset = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE,selectedTextureAsset).value().get();
          parent.pushPropertyTarget(PropertyType_Asset,asset);
          return;
        }

        ImGui::SameLine();

        if(ImGuiWidget::StringCombo("Sprite",&selectedTextureAsset,textureAssets))
        {
          entDef.setSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, selectedTextureAsset);
        }
      }
    }
  }
}
