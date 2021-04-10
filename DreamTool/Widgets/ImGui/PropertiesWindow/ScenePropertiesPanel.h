#pragma once

#include "Widgets/ImGui/ImGuiPanel.h"
#include "IdentityPropertiesPanel.h"
#include "TransformPropertiesPanel.h"

namespace octronic::dream::tool
{
	class ScenePropertiesPanel : public ImGuiPanel
  {
  public:
    ScenePropertiesPanel(ImGuiWidget& parent);
    void draw() override;
  private:
    bool drawDeleteSceneButton();
  private:
    IdentityPropertiesPanel mNameAndUuidPanel;
    TransformPropertiesPanel mCameraTransformPanel;
  };
}
