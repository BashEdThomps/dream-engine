#include "ToolPropertiesWindow.h"
#include "../../DTState.h"
#include <glm/gtc/type_ptr.hpp>

namespace DreamTool
{
    ToolPropertiesWindow::ToolPropertiesWindow
    (DTState* p)
        : ImGuiWidget(p,false)
    {
#ifdef DREAM_LOG
        setLogClassName("GridPropertiesWidget");
#endif
    }

    ToolPropertiesWindow::~ToolPropertiesWindow()
    {

    }

    void ToolPropertiesWindow::draw()
    {
        if (mState->project)
        {
            ImGui::Begin("Tools",&mVisible);

            ImGui::Text("Selection");
            bool highlightSelected = mState->selectionHighlighter.getVisible();
            if (ImGui::Checkbox("Highlight Selected",&highlightSelected))
            {
                mState->selectionHighlighter.setVisible(highlightSelected);
            }
            ImGui::Separator();

            ImGui::Text("Cursor");

            bool showCursor = mState->cursor.getVisible();
            if(ImGui::Checkbox("Show Cursor",&showCursor))
            {
                mState->cursor.setVisible(showCursor);
            }

            ImGui::SameLine();

            static bool snapToGrid = true;
            ImGui::Checkbox("Snap to Grid",&snapToGrid);
            ImGui::PushItemWidth(-1);
            float speed = 1.0f/mState->grid.getMinorSpacing();
            vec3 cursorPosVec = mState->cursor.getPosition();
            if(ImGui::DragFloat3("##",glm::value_ptr(cursorPosVec),speed,speed,speed))
            {
                mState->cursor.setPosition(cursorPosVec,snapToGrid);
            }
            ImGui::PopItemWidth();

            ImGui::Separator();
            bool showLights = mState->lightViewer.getVisible();
            if(ImGui::Checkbox("Show Light Markers",&showLights))
            {
                mState->lightViewer.setVisible(showLights);
            }
            ImGui::Separator();

            bool showGrid = mState->grid.getVisible();
            if (ImGui::Checkbox("Show Grid",&showGrid))
            {
                mState->grid.setVisible(showGrid);
            }

            ImGui::Separator();
            auto gridTx = mState->grid.getTranslation();
            float pos[3] = {
                gridTx.x,
                gridTx.y,
                gridTx.z
            };
            if(ImGui::DragFloat3("Position",pos,mState->grid.getMinorSpacing()))
            {
                mState->grid.setTranslation(vec3(pos[0],pos[1],pos[2]));
            }
            ImGui::Separator();
            float size = mState->grid.getSize();
            if(ImGui::DragFloat("Size",&size,2.0f))
            {
                mState->grid.setSize(size);
                mState->grid.recalculateGridLines();
            }

            float maj = mState->grid.getMajorSpacing();
            if(ImGui::DragFloat("Major Spacing",&maj,1.0f))
            {
                mState->grid.setMajorSpacing(maj);
                mState->grid.recalculateGridLines();
            }
            float min = mState->grid.getMinorSpacing();
            if(ImGui::DragFloat("Minor Spacing",&min,1.0f))
            {
                mState->grid.setMinorSpacing(min);
                mState->grid.recalculateGridLines();
            }

            ImGui::Separator();

            vec3 majColorVec = mState->grid.getMajorColour();
            float majColor[3] = {
                majColorVec.x,
                majColorVec.y,
                majColorVec.z,
            };

            if (ImGui::ColorEdit3("Major Color",majColor))
            {
                mState->grid.setMajorColour(vec3(majColor[0],majColor[1],majColor[2]));
                mState->grid.recalculateGridLines();
            }

            vec3 minColorVec = mState->grid.getMinorColour();
            float minColor[3] = {
                minColorVec.x,
                minColorVec.y,
                minColorVec.z,
            };

            if (ImGui::ColorEdit3("Minor Color",minColor))
            {
                mState->grid.setMinorColour(vec3(minColor[0],minColor[1],minColor[2]));
                mState->grid.recalculateGridLines();
            }
            ImGui::Separator();

            vector<string> orientations = {"XY","XZ","YZ"};
            int orientationIndex = mState->grid.getAxisPair();
            if (StringCombo("Orientation",&orientationIndex,orientations,orientations.size()))
            {
                Grid::AxisPair ap = static_cast<Grid::AxisPair>(orientationIndex);
                mState->grid.setAxisPair(ap);
                mState->cursor.onAxisPairChanged(ap);
                mState->grid.recalculateGridLines();
            }
            ImGui::End();
        }
    }
}
