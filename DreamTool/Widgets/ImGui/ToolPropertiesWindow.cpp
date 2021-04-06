#include "ToolPropertiesWindow.h"
#include "DreamToolContext.h"
#include <glm/gtc/type_ptr.hpp>

namespace octronic::dream::tool
{
  ToolPropertiesWindow::ToolPropertiesWindow
  (DreamToolContext& p)
    : ImGuiWidget(p,true)
  {
  }

  ToolPropertiesWindow::~ToolPropertiesWindow()
  {

  }

  void ToolPropertiesWindow::drawSelectionSettings()
  {
    auto& ctx = getContext();
    bool highlightSelected = ctx.getSelectionHighlighter().getVisible();
    if (ImGui::Checkbox("Highlight Selected",&highlightSelected))
    {
      ctx.getSelectionHighlighter().setVisible(highlightSelected);
    }
  }

  void ToolPropertiesWindow::drawCursorSettings()
  {
    ImGui::PushID("CursorSettings");
    auto& ctx = getContext();
    auto& cursor = ctx.getCursor();

    bool showCursor = cursor.getVisible();

    if(ImGui::Checkbox("Show Cursor",&showCursor))
    {
      cursor.setVisible(showCursor);
    }

    static bool snapToGrid = true;
    ImGui::Checkbox("Snap Cursor to Grid",&snapToGrid);

    Transform cursorPosTx = cursor.getTransform();

    vec3 cursorPos = cursorPosTx.getTranslation();
    if(ImGui::DragFloat3("Position",glm::value_ptr(cursorPos)))
    {
      cursorPosTx.setTranslation(cursorPos);
      cursor.setTransform(cursorPosTx);
    }
    ImGui::PopID();
  }

  void ToolPropertiesWindow::drawLightIndicatorSettings()
  {
    ImGui::PushID("LightSettings");
    auto& ctx = getContext();
    auto& lightViewer = ctx.getLightViewer();
    bool showLights =  lightViewer.getVisible();
    if(ImGui::Checkbox("Show Light Markers",&showLights))
    {
      lightViewer.setVisible(showLights);
    }
    ImGui::Separator();
    ImGui::Columns(3);
    ImGui::Text("Index");
    ImGui::NextColumn();
    ImGui::Text("Position");
    ImGui::NextColumn();
    ImGui::Text("Color");
    ImGui::NextColumn();
    ImGui::Separator();

    auto& pCtxOpt = ctx.getProjectContext();

    if (pCtxOpt)
    {
      auto& pCtx = pCtxOpt.value();
      auto& pRuntOpt = pCtx.getProjectRuntime();

      if (pRuntOpt)
      {
        auto& pRunt = pRuntOpt.value();
        auto& gc = pRunt.getGraphicsComponent();

        size_t num_lights = gc.getLightCount();
        for (size_t i=0; i< num_lights; i++)
        {
          ImGui::PushID(i);
          vec3 pos = gc.getLightPosition(i);
          vec3 color = gc.getLightColor(i);
          ImGui::Text("%lu", i);
          ImGui::NextColumn();

          ImGui::PushItemWidth(-1);
          if(ImGui::DragFloat3("##Position",glm::value_ptr(pos)))
          {
            gc.setLightPosition(i, pos);
          }
          ImGui::PopItemWidth();
          ImGui::NextColumn();

          ImGui::PushItemWidth(-1);
          if (ImGui::DragFloat3("##Color",glm::value_ptr(color)))
          {
            gc.setLightColor(i,color);
          }
          ImGui::PopItemWidth();
          ImGui::NextColumn();
          ImGui::PopID();
        }
      }
    }
    ImGui::Separator();
    ImGui::PopID();
  }

  void ToolPropertiesWindow::drawGridSettings()
  {
    auto& ctx = getContext();
    auto& grid = ctx.getGrid();

    ImGui::PushID("GridSettings");
    bool showGrid = grid.getVisible();
    if (ImGui::Checkbox("Show Grid",&showGrid))
    {
      grid.setVisible(showGrid);
    }

    ImGui::Separator();

    vec3 gridTx = grid.getTransform().getTranslation();

    if(ImGui::DragFloat3("Position",glm::value_ptr(gridTx)), grid.getMinorSpacing())
    {
      auto transform = grid.getTransform();
      transform.setTranslation(gridTx);
      grid.setTransform(transform);
    }

    ImGui::Separator();

    float size = grid.getSize();
    if(ImGui::DragFloat("Size",&size,2.0f))
    {
      grid.setSize(size);
      grid.recalculateGridLines();
    }

    float maj = grid.getMajorSpacing();
    if(ImGui::DragFloat("Major Spacing",&maj,1.0f))
    {
      grid.setMajorSpacing(maj);
      grid.recalculateGridLines();
    }

    float min = grid.getMinorSpacing();
    if(ImGui::DragFloat("Minor Spacing",&min,.1f))
    {
      grid.setMinorSpacing(min);
      grid.recalculateGridLines();
    }

    ImGui::Separator();

    vec4 majColorVec = grid.getMajorColour();

    if (ImGui::ColorEdit4("Major Color",glm::value_ptr(majColorVec)))
    {
      grid.setMajorColour(majColorVec);
      grid.recalculateGridLines();
    }

    vec4 minColorVec = grid.getMinorColour();

    if (ImGui::ColorEdit4("Minor Color",glm::value_ptr(minColorVec)))
    {
      grid.setMinorColour(minColorVec);
      grid.recalculateGridLines();
    }
    ImGui::Separator();

    vector<string> orientations = {"XY","XZ","YZ"};
    int orientationIndex = grid.getAxisPair();
    if (StringCombo("Orientation",&orientationIndex,orientations,orientations.size()))
    {
      Grid::AxisPair ap = static_cast<Grid::AxisPair>(orientationIndex);
      auto& cursor = ctx.getCursor();
      grid.setAxisPair(ap);
      cursor.onAxisPairChanged(ap);
      grid.recalculateGridLines();
    }
    ImGui::PopID();
  }

  void ToolPropertiesWindow::draw()
  {
    auto& ctx = getContext();
    auto& pCtxOpt = ctx.getProjectContext();
    if (pCtxOpt)
    {
      ImGui::Begin("Tool Settings",&mVisible);
      drawSelectionSettings();
      ImGui::Separator();
      drawCursorSettings();
      ImGui::Separator();
      drawGridSettings();
      ImGui::Separator();
      drawLightIndicatorSettings();
      ImGui::End();
    }
  }
}
