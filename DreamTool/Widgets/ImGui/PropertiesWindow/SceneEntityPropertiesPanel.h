#pragma once

#include "Widgets/ImGui/ImGuiPanel.h"

#include "TransformPropertiesPanel.h"
#include "IdentityPropertiesPanel.h"

namespace octronic::dream::tool
{
  class SceneEntityPropertiesPanel : public ImGuiPanel
  {
  public:
    SceneEntityPropertiesPanel(ImGuiWidget& parent);
    void draw() override;
  protected:
    void drawTextProperties();
  private:
    IdentityPropertiesPanel mNameAndUuidPanel;
    TransformPropertiesPanel mTransformPropertiesPanel;
  };
}

