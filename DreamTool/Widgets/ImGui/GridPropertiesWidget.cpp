#include "GridPropertiesWidget.h"
#include "../GL/Grid.h"

namespace DreamTool
{
    GridPropertiesWindow::GridPropertiesWindow
    (Project* p)
        : ImGuiWidget(p),
          mGridHandle(nullptr)
    {
       setLogClassName("GridPropertiesWidget");
    }

    GridPropertiesWindow::~GridPropertiesWindow()
    {

    }


    void GridPropertiesWindow::draw()
    {
        ImGui::Begin("Grid Properties");
        bool showGrid = !mGridHandle->getHidden();
        if (ImGui::Checkbox("Show Grid",&showGrid))
        {
            mGridHandle->setHidden(!showGrid);
        }

        ImGui::Separator();
        auto gridTx = mGridHandle->getTranslation();
        float pos[3] = {
            gridTx.x,
            gridTx.y,
            gridTx.z
        };
        if(ImGui::DragFloat3("Position",pos,mGridHandle->getMinorSpacing()))
        {
            mGridHandle->setTranslation(vec3(pos[0],pos[1],pos[2]));
        }
        ImGui::Separator();
        float maj = mGridHandle->getMajorSpacing();
        if(ImGui::InputFloat("Major Spacing",&maj))
        {
            mGridHandle->setMajorSpacing(maj);
            mGridHandle->recalculateGridLines();
        }
        float min = mGridHandle->getMinorSpacing();
        if(ImGui::InputFloat("Minor Spacing",&min))
        {
            mGridHandle->setMinorSpacing(min);
            mGridHandle->recalculateGridLines();
        }

        ImGui::Separator();

        vec3 majColorVec = mGridHandle->getMajorColour();
        float majColor[3] = {
            majColorVec.x,
            majColorVec.y,
            majColorVec.z,
        };

        if (ImGui::ColorEdit3("Major Color",majColor))
        {
           mGridHandle->setMajorColour(vec3(majColor[0],majColor[1],majColor[2]));
           mGridHandle->recalculateGridLines();
        }

        vec3 minColorVec = mGridHandle->getMinorColour();
        float minColor[3] = {
            minColorVec.x,
            minColorVec.y,
            minColorVec.z,
        };

        if (ImGui::ColorEdit3("Minor Color",minColor))
        {
           mGridHandle->setMinorColour(vec3(minColor[0],minColor[1],minColor[2]));
           mGridHandle->recalculateGridLines();
        }
        ImGui::Separator();

        vector<string> orientations = {"XY","XZ","YZ"};
        int orientationIndex = mGridHandle->getAxisPair();
        if (StringCombo("Orientation",&orientationIndex,orientations,orientations.size()))
        {
            mGridHandle->setAxisPair(static_cast<Grid::AxisPair>(orientationIndex));
            mGridHandle->recalculateGridLines();
        }

        ImGui::Separator();

        static bool snapToGrid = false;
        if (ImGui::Checkbox("Snap to Grid",&snapToGrid))
        {

        }

        ImGui::End();
    }

    void
    GridPropertiesWindow::setGrid
    (Grid* gridHandle)
    {
        mGridHandle = gridHandle;
    }
}
