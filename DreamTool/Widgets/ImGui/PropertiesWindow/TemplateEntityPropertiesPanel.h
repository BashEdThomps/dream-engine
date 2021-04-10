#pragma once

#include "Widgets/ImGui/ImGuiPanel.h"
#include "IdentityPropertiesPanel.h"

namespace octronic::dream::tool
{
  class TemplateEntityPropertiesPanel : public ImGuiPanel
  {
  public:
    TemplateEntityPropertiesPanel(ImGuiWidget& parent);
    void draw() override;
  protected:
  	void drawTransformProperties();
    void drawTextProperties();
    void drawAnimationTransport();
    void drawAssetSelection();
  private:
    IdentityPropertiesPanel mNameAndUuidPanel;
  };
}

