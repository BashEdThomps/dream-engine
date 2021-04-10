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

    if (!parent.getCurrentPropertyTarget().has_value()) return;

    auto target = parent.getCurrentPropertyTarget().value().get();

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
      if (ImGui::Button("Delete"))
      {
        auto& parentDef = entityDef.getParentDefinition();

        dtCtx.getProjectBrowser().removeNodeSelection(entityDef);
        dtCtx.getSelectionHighlighter().clearSelection();
        parent.removeFromHistoryWithChildren(entityDef);

        if (parentDef.has_value())
        {
          parentDef.value().get().removeChildDefinition(entityDef);
        }
        return;
      }
    }

    ImGui::SameLine();

    if (ImGui::Button("Add Child"))
    {
      auto& cursor = dtCtx.getCursor();
      auto& newChildDef = entityDef.createChildDefinition();
      newChildDef.setTransform(cursor.getTransform());
      parent.pushPropertyTarget(PropertyType_SceneEntity,newChildDef);
    }

    ImGui::SameLine();

    // Not root
    if (entityDef.hasParentDefinition())
    {
      if (ImGui::Button("Duplicate"))
      {
        auto& dupe = entityDef.duplicateDefinition();
        parent.pushPropertyTarget(PropertyType_SceneEntity, dupe);
      }
    }

    mNameAndUuidPanel.draw();

    if (ImGui::Button("Use as Camera"))
    {
      auto& camDef = sceneDef.getCameraDefinition();
      camDef.setCameraEntityUuid(entityDef.getUuid());
      camDef.setUseEntity(true);
    }

    ImGui::Separator();

    if (ImGui::Button("-"))
    {
      entityDef.setTemplateUuid(Uuid::INVALID);
    }

    ImGui::SameLine();

    auto templateDefOpt = pDef.getTemplateEntityDefinitionByUuid(entityDef.getTemplateUuid());

    int templateDefIndex = -1;
    if (templateDefOpt)
    {
      auto& templateDef = templateDefOpt.value().get();
      templateDefIndex = pDef.getTemplateEntityDefinitionIndex(templateDef);
    }

    vector<string> templateEntityDefNames = pDef.getTemplateEntityDefinitionNamesVector();
    if (ImGuiWidget::StringCombo("Entity Template", &templateDefIndex, templateEntityDefNames))
    {
      auto& selected = pDef.getTemplateEntityDefinitionAtIndex(templateDefIndex);
      entityDef.setTemplateUuid(selected.getUuid());
    }

    auto tx = entityDef.getTransform();
    mTransformPropertiesPanel.setTransform(tx);
    mTransformPropertiesPanel.draw();
    entityDef.setTransform(tx);

    if (ImGui::Button("Apply to Runtime"))
    {
      if (pCtx.getProjectRuntime().has_value())
      {
        auto& pRunt = pCtx.getProjectRuntime().value();
        if (pRunt.getSceneRuntimeByUuid(sceneDef.getUuid()))
        {
          auto& sRunt = pRunt.getSceneRuntimeByUuid(sceneDef.getUuid()).value().get();
          if (sRunt.getEntityRuntimeByUuid(entityDef.getUuid()))
          {
            auto entityRuntOpt =  sRunt.getEntityRuntimeByUuid(entityDef.getUuid());
            if (entityRuntOpt)
            {
              auto& entityRunt = entityRuntOpt.value().get();
                  entityRunt.setTransform(tx);
            }
          }
        }
      }
    }
  }

  void
  SceneEntityPropertiesPanel::drawTextProperties
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());

    if (!parent.getCurrentPropertyTarget().has_value()) return;

    auto target = parent.getCurrentPropertyTarget().value().get();

    auto& entityDef = static_cast<SceneEntityDefinition&>(target.getDefinition().value().get());

    ImGui::Separator();

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
