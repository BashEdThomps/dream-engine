#pragma once

#include "Widgets/ImGui/ImGuiPanel.h"
#include "NameAndUuidPanel.h"

namespace octronic::dream::tool
{
  class AssetPropertiesPanel : public ImGuiPanel
  {
  public:
    AssetPropertiesPanel(ImGuiWidget& parent);
    void draw() override;
  protected:
    void drawAnimationAssetProperties();
    void drawAudioAssetProperties();
    void drawFontAssetProperties();
    void drawMaterialAssetProperties();
    void drawModelAssetProperties();
    void drawPhysicsObjectAssetProperties();
    void drawScriptProperties();
    void drawShaderAssetProperties();
    void drawPathAssetProperties();
    void drawTextureAssetProperties();
  private:
    NameAndUuidPanel mNameAndUuidPanel;
  };
}

