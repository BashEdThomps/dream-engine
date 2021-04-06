#pragma once

#include "Widgets/ImGui/ImGuiPanel.h"

#include "TransformPropertiesPanel.h"
#include "NameAndUuidPanel.h"

namespace octronic::dream::tool
{
  class SceneEntityPropertiesPanel : public ImGuiPanel
  {
  public:
    SceneEntityPropertiesPanel(ImGuiWidget& parent);
    void draw() override;
  protected:
  	bool drawDeleteEntityButton();
  	void drawTransformProperties();
    void drawTextProperties();
  private:
    NameAndUuidPanel mNameAndUuidPanel;
    TransformPropertiesPanel mTransformPropertiesPanel;
  };
}

