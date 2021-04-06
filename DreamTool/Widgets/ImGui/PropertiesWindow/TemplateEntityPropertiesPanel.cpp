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
    auto target = parent.getCurrentPropertyTarget();
    auto entDefOpt = target.getDefinition();

    // Valid Definition
    if (!entDefOpt)
    {
      return;
    }

    auto& entDef = static_cast<TemplateEntityDefinition&>(entDefOpt.value().get());

    ImGui::SameLine();
    if (drawDeleteEntityButton()) return;

    ImGui::SameLine();

    ImGui::SameLine();
    if (ImGui::Button("Duplicate"))
    {
      auto& dupe = entDef.duplicateDefinition();
      parent.pushPropertyTarget({PropertyType_TemplateEntity, dupe});
    }

    mNameAndUuidPanel.draw();

    ImGui::Separator();

    drawAssetSelection();
  }

  bool
  TemplateEntityPropertiesPanel::drawDeleteEntityButton
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    auto& dtCtx = parent.getContext();
    auto& pCtx = dtCtx.getProjectContext().value();
    auto& pDef = pCtx.getProjectDefinition().value();
    auto target = parent.getCurrentPropertyTarget();
    auto& entDef = static_cast<TemplateEntityDefinition&>(target.getDefinition().value().get());

    if (ImGui::Button("Delete"))
    {
      pDef.removeTemplateEntityDefinition(entDef);
      parent.removeFromHistory(entDef);
      return true;
    }
    return false;
  }

  void
  TemplateEntityPropertiesPanel::drawAssetSelection
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    auto& dtCtx = parent.getContext();
    auto& pCtx = dtCtx.getProjectContext().value();
    auto& pDef = pCtx.getProjectDefinition().value();
    auto target = parent.getCurrentPropertyTarget();
    auto& entDef = static_cast<TemplateEntityDefinition&>(target.getDefinition().value().get());

    ImGui::Text("Assets");

    // Animation =======================================================
    {
      ImGui::Text("Animation");

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
        parent.pushPropertyTarget({PropertyType_Asset,asset});
        return;
      }

      ImGui::SameLine();

      ImGui::PushItemWidth(-1);
      if(ImGuiWidget::StringCombo("##AnimationCombo",&selectedAnimationAsset,animationAssets,animationAssets.size()))
      {
        entDef.setSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_ANIMATION, selectedAnimationAsset);
      }
      ImGui::PopItemWidth();
    }

    // Audio ===========================================================
    {
      ImGui::Text("Audio");

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
        parent.pushPropertyTarget({PropertyType_Asset,asset});
        return;
      }

      ImGui::SameLine();

      ImGui::PushItemWidth(-1);
      if(ImGuiWidget::StringCombo("##AudioCombo",&selectedAudioAsset,audioAssets,audioAssets.size()))
      {
        entDef.setSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_AUDIO, selectedAudioAsset);
      }
      ImGui::PopItemWidth();
    }

    // Font ============================================================
    {
      ImGui::Text("Font");

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
        parent.pushPropertyTarget({PropertyType_Asset,asset});
        return;
      }

      ImGui::SameLine();

      ImGui::PushItemWidth(-1);
      if(ImGuiWidget::StringCombo("##FontCombo",&selectedFontAsset,fontAssets,fontAssets.size()))
      {
        entDef.setSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_FONT, selectedFontAsset);
      }
      ImGui::PopItemWidth();
    }

    // Model ===========================================================
    {
      ImGui::Text("Model");
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
        parent.pushPropertyTarget({PropertyType_Asset,asset});
        return;
      }

      ImGui::SameLine();

      ImGui::PushItemWidth(-1);
      if(ImGuiWidget::StringCombo("##ModelCombo",&selectedModelAsset,modelAssets,modelAssets.size()))
      {
        entDef.setSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_MODEL, selectedModelAsset);
      }
      ImGui::PopItemWidth();
    }

    // Path ============================================================
    {
      ImGui::Text("Path");
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
        parent.pushPropertyTarget({PropertyType_Asset, asset});
        return;
      }

      ImGui::SameLine();

      ImGui::PushItemWidth(-1);
      if(ImGuiWidget::StringCombo("##PathCombo",&selectedPathAsset,pathAssets,pathAssets.size()))
      {
        entDef.setSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_PATH, selectedPathAsset);
      }
      ImGui::PopItemWidth();
    }

    // Physics Object ==================================================
    {
      ImGui::Text("Physics Object");
      int selectedPhysicsObjectAsset = entDef.getSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT);
      vector<string> poAssets = pDef.getAssetNamesVector(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT);
      if(ImGui::Button("-##PhysicsObject"))
      {
        entDef.setAssetDefinition(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT,Uuid::INVALID);
      }
      ImGui::SameLine();

      if(ImGui::Button(">##PhysicsObject"))
      {
        if (selectedPhysicsObjectAsset < 0) return;
        auto& asset = pDef.getAssetDefinitionAtIndex(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT,selectedPhysicsObjectAsset).value().get();
        parent.pushPropertyTarget({PropertyType_Asset, asset});
        return;
      }

      ImGui::SameLine();

      ImGui::PushItemWidth(-1);
      if(ImGuiWidget::StringCombo("##PhysicsObjectCombo",&selectedPhysicsObjectAsset,poAssets,poAssets.size()))
      {
        entDef.setSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_PHYSICS_OBJECT,selectedPhysicsObjectAsset);
      }
      ImGui::PopItemWidth();
    }

    // Script ==========================================================
    {
      ImGui::Text("Script");
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
        parent.pushPropertyTarget({PropertyType_Asset,asset});
        return;
      }

      ImGui::SameLine();

      ImGui::PushItemWidth(-1);
      if(ImGuiWidget::StringCombo("##ScriptCombo",&selectedScriptAsset,scriptAssets,scriptAssets.size()))
      {
        entDef.setSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_SCRIPT, selectedScriptAsset);
      }
      ImGui::PopItemWidth();
    }

    // Texture (Sprite) ================================================
    {
      ImGui::Text("Sprite Texture");
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
        parent.pushPropertyTarget({PropertyType_Asset,asset});
        return;
      }

      ImGui::SameLine();

      ImGui::PushItemWidth(-1);
      if(ImGuiWidget::StringCombo("##TextureCombo",&selectedTextureAsset,textureAssets,textureAssets.size()))
      {
        entDef.setSelectedAssetDefinitionIndex(AssetType::ASSET_TYPE_ENUM_TEXTURE, selectedTextureAsset);
      }
      ImGui::PopItemWidth();
    }
  }
}
