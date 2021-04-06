#include "SceneEntityPropertiesPanel.h"

#include "PropertiesWindow.h"
#include "DreamToolContext.h"

using std::make_optional;

namespace octronic::dream::tool
{
  SceneEntityPropertiesPanel::SceneEntityPropertiesPanel
  (ImGuiWidget& parent)
    : ImGuiPanel(parent),
      mNameAndUuidPanel(parent),
      mTransformPropertiesPanel(parent)
  {

  }

  void
  SceneEntityPropertiesPanel::draw
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    auto& dtCtx = parent.getContext();
    auto& pCtx = dtCtx.getProjectContext().value();
    auto& pDef = pCtx.getProjectDefinition().value();
    auto target = parent.getCurrentPropertyTarget();
    auto entityDefOpt = target.getDefinition();


    // Valid Definition
    if (!entityDefOpt)
    {
      return;
    }

    auto& entityDef = static_cast<SceneEntityDefinition&>(entityDefOpt.value().get());
    auto& sceneDef = entityDef.getSceneDefinition();

    // Not the root
    if (entityDef.hasParentDefinition())
    {
      ImGui::SameLine();
      if (drawDeleteEntityButton())
      {
        return;
      }
    }

    ImGui::SameLine();

    if (ImGui::Button("Add Child"))
    {
      auto cursor = dtCtx.getCursor();
      auto& newChildDef = entityDef.createChildDefinition();
      newChildDef.setTransform(cursor.getTransform());
      parent.pushPropertyTarget({PropertyType_SceneEntity,newChildDef});
    }

    // Not root
    if (entityDef.hasParentDefinition())
    {
      ImGui::SameLine();
      if (ImGui::Button("Duplicate"))
      {
        auto& dupe = entityDef.duplicateDefinition();
        parent.pushPropertyTarget({PropertyType_SceneEntity, dupe});
      }
    }

    mNameAndUuidPanel.draw();

    ImGui::Separator();

    ImGui::SameLine();

    if (ImGui::Button("Use as Camera"))
    {
      auto& camDef = sceneDef.getCameraDefinition();
      camDef.setCameraEntityUuid(entityDef.getUuid());
      camDef.setUseEntity(true);
    }

    auto templateDefOpt = pDef.getTemplateEntityDefinitionByUuid(entityDef.getTemplateUuid());

    if (templateDefOpt)
    {
      auto& templateDef = templateDefOpt.value().get();

      if (templateDef.getAssetDefinition(AssetType::ASSET_TYPE_ENUM_FONT) != Uuid::INVALID)
      {
        ImGui::Separator();
        drawTextProperties();
      }

      if (templateDef.getAssetDefinition(ASSET_TYPE_ENUM_ANIMATION) != Uuid::INVALID)
      {
      }
    }

    mTransformPropertiesPanel.draw();

  }

  bool
  SceneEntityPropertiesPanel::drawDeleteEntityButton
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    auto& dtCtx = parent.getContext();
    auto target = parent.getCurrentPropertyTarget();
    auto& entityDef = static_cast<SceneEntityDefinition&>(target.getDefinition().value().get());

    if (ImGui::Button("Delete"))
    {
      auto& parentDef = entityDef.getParentDefinition();
      if (parentDef.has_value())
      {
      	parentDef.value().get().removeChildDefinition(entityDef);
      }

      dtCtx.getSelectionHighlighter().clearSelection();
      parent.removeFromHistory(entityDef);
      return true;
    }
    return false;
  }

  void
  SceneEntityPropertiesPanel::drawTextProperties
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    auto& dtCtx = parent.getContext();
    auto target = parent.getCurrentPropertyTarget();
    auto& entityDef = static_cast<SceneEntityDefinition&>(target.getDefinition().value().get());

    ImGui::Text("Text Properties");
    char fontTextBuffer[128] = {0};

    string fontText = entityDef.getFontText();
    strncpy(fontTextBuffer, fontText.c_str(), fontText.size());

    if (ImGui::InputText("Text", fontTextBuffer,128))
    {
      string txt(fontTextBuffer);
      entityDef.setFontText(txt);
    }

    vec4 fontColorV = entityDef.getFontColor();
    float fontColor[4] = {fontColorV.r, fontColorV.g, fontColorV.b, fontColorV.a};

    if (ImGui::ColorPicker4("Color", &fontColor[0]))
    {
      fontColorV.r = fontColor[0];
      fontColorV.g = fontColor[1];
      fontColorV.b = fontColor[2];
      fontColorV.a = fontColor[3];
      entityDef.setFontColor(fontColorV);

    }

    float fontScale = entityDef.getFontScale();
    if (ImGui::InputFloat("Font Scale",&fontScale))
    {
      entityDef.setFontScale(fontScale);
    }

    ImGui::Separator();
  }
}
