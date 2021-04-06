#pragma once

#include "Widgets/ImGui/ImGuiPanel.h"
#include "NameAndUuidPanel.h"

namespace octronic::dream::tool
{
  class TemplateEntityPropertiesPanel : public ImGuiPanel
  {
  public:
    TemplateEntityPropertiesPanel(ImGuiWidget& parent);
    void draw() override;
  protected:
  	bool drawDeleteEntityButton();
  	void drawTransformProperties();
    void drawTextProperties();
    void drawAnimationTransport();
    void drawAssetSelection();
  private:
    NameAndUuidPanel mNameAndUuidPanel;
  };
}

