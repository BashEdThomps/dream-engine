#include "ToolPropertiesWindow.h"
#include "DreamToolContext.h"
#include <glm/gtc/type_ptr.hpp>

namespace octronic::dream::tool
{
    ToolPropertiesWindow::ToolPropertiesWindow
    (DreamToolContext* p)
        : ImGuiWidget(p,false)
    {
    }

    ToolPropertiesWindow::~ToolPropertiesWindow()
    {

    }

    void ToolPropertiesWindow::draw()
    {
        if (mContext->getProject())
        {
            ImGui::Begin("Tools",&mVisible);

            ImGui::Text("Selection");
            bool highlightSelected = mContext->getSelectionHighlighter()->getVisible();
            if (ImGui::Checkbox("Highlight Selected",&highlightSelected))
            {
                mContext->getSelectionHighlighter()->setVisible(highlightSelected);
            }
            ImGui::Separator();

            ImGui::Text("Cursor");

            bool showCursor = mContext->getCursor()->getVisible();
            if(ImGui::Checkbox("Show Cursor",&showCursor))
            {
                mContext->getCursor()->setVisible(showCursor);
            }

            ImGui::SameLine();

            static bool snapToGrid = true;
            ImGui::Checkbox("Snap to Grid",&snapToGrid);
            ImGui::PushItemWidth(-1);
            float speed = 1.0f/mContext->getGrid()->getMinorSpacing();
            vec3 cursorPosVec = mContext->getCursor()->getPosition();
            if(ImGui::DragFloat3("##",glm::value_ptr(cursorPosVec),speed,speed,speed))
            {
                mContext->getCursor()->setPosition(cursorPosVec,snapToGrid);
            }
            ImGui::PopItemWidth();

            ImGui::Separator();
            bool showLights = mContext->getLightViewer()->getVisible();
            if(ImGui::Checkbox("Show Light Markers",&showLights))
            {
                mContext->getLightViewer()->setVisible(showLights);
            }
            ImGui::Separator();

            bool showGrid = mContext->getGrid()->getVisible();

            if (ImGui::Checkbox("Show Grid",&showGrid))
            {
                mContext->getGrid()->setVisible(showGrid);
            }

            ImGui::Separator();
            auto gridTx = mContext->getGrid()->getTranslation();
            float pos[3] = {
                gridTx.x,
                gridTx.y,
                gridTx.z
            };
            if(ImGui::DragFloat3("Position",pos,mContext->getGrid()->getMinorSpacing()))
            {
                mContext->getGrid()->setTranslation(vec3(pos[0],pos[1],pos[2]));
            }
            ImGui::Separator();
            float size = mContext->getGrid()->getSize();
            if(ImGui::DragFloat("Size",&size,2.0f))
            {
                mContext->getGrid()->setSize(size);
                mContext->getGrid()->recalculateGridLines();
            }

            float maj = mContext->getGrid()->getMajorSpacing();
            if(ImGui::DragFloat("Major Spacing",&maj,1.0f))
            {
                mContext->getGrid()->setMajorSpacing(maj);
                mContext->getGrid()->recalculateGridLines();
            }
            float min = mContext->getGrid()->getMinorSpacing();
            if(ImGui::DragFloat("Minor Spacing",&min,1.0f))
            {
                mContext->getGrid()->setMinorSpacing(min);
                mContext->getGrid()->recalculateGridLines();
            }

            ImGui::Separator();

            vec3 majColorVec = mContext->getGrid()->getMajorColour();
            float majColor[3] = {
                majColorVec.x,
                majColorVec.y,
                majColorVec.z,
            };

            if (ImGui::ColorEdit3("Major Color",majColor))
            {
                mContext->getGrid()->setMajorColour(vec3(majColor[0],majColor[1],majColor[2]));
                mContext->getGrid()->recalculateGridLines();
            }

            vec3 minColorVec = mContext->getGrid()->getMinorColour();
            float minColor[3] = {
                minColorVec.x,
                minColorVec.y,
                minColorVec.z,
            };

            if (ImGui::ColorEdit3("Minor Color",minColor))
            {
                mContext->getGrid()->setMinorColour(vec3(minColor[0],minColor[1],minColor[2]));
                mContext->getGrid()->recalculateGridLines();
            }
            ImGui::Separator();

            vector<string> orientations = {"XY","XZ","YZ"};
            int orientationIndex = mContext->getGrid()->getAxisPair();
            if (StringCombo("Orientation",&orientationIndex,orientations,orientations.size()))
            {
                Grid::AxisPair ap = static_cast<Grid::AxisPair>(orientationIndex);
                mContext->getGrid()->setAxisPair(ap);
                mContext->getCursor()->onAxisPairChanged(ap);
                mContext->getGrid()->recalculateGridLines();
            }
            ImGui::End();
        }
    }
}
