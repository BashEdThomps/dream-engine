#pragma once

#include "Widgets/ImGui/ImGuiPanel.h"
#include "NameAndUuidPanel.h"
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
    NameAndUuidPanel mNameAndUuidPanel;
    TransformPropertiesPanel mCameraTransformPanel;
  };
}
