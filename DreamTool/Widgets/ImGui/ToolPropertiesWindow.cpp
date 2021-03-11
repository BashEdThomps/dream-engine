#include "ToolPropertiesWindow.h"
#include "DreamToolContext.h"
#include <glm/gtc/type_ptr.hpp>

namespace octronic::dream::tool
{
    ToolPropertiesWindow::ToolPropertiesWindow
    (DreamToolContext* p)
        : ImGuiWidget(p,true)
    {
    }

    ToolPropertiesWindow::~ToolPropertiesWindow()
    {

    }

    void ToolPropertiesWindow::drawSelectionSettings()
    {
        bool highlightSelected = mContext->getSelectionHighlighter()->getVisible();
        if (ImGui::Checkbox("Highlight Selected",&highlightSelected))
        {
            mContext->getSelectionHighlighter()->setVisible(highlightSelected);
        }
    }

    void ToolPropertiesWindow::drawCursorSettings()
    {
        ImGui::PushID("CursorSettings");
        bool showCursor = mContext->getCursor()->getVisible();
        if(ImGui::Checkbox("Show Cursor",&showCursor))
        {
            mContext->getCursor()->setVisible(showCursor);
        }

        static bool snapToGrid = true;
        ImGui::Checkbox("Snap Cursor to Grid",&snapToGrid);

        Transform cursorPosTx = mContext->getCursor()->getTransform();

        vec3 cursorPos = cursorPosTx.getTranslation();
        if(ImGui::DragFloat3("Position",glm::value_ptr(cursorPos)))
        {
            cursorPosTx.setTranslation(cursorPos);
            mContext->getCursor()->setTransform(cursorPosTx);
        }
        ImGui::PopID();
    }

    void ToolPropertiesWindow::drawLightIndicatorSettings()
    {
        ImGui::PushID("LightSettings");
        bool showLights =  mContext->getLightViewer()->getVisible();
        if(ImGui::Checkbox("Show Light Markers",&showLights))
        {
            mContext->getLightViewer()->setVisible(showLights);
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

        auto project = mContext->getProject();
        if (project)
        {
            auto projectRuntime = project->getRuntime();
            if (projectRuntime)
            {
                auto gc = projectRuntime->getGraphicsComponent();
                if (gc)
                {
                    size_t num_lights = gc->getLightCount();
                    for (size_t i=0; i< num_lights; i++)
                    {
                        ImGui::PushID(i);
                        vec3 pos = gc->getLightPosition(i);
                        vec3 color = gc->getLightColor(i);
                        ImGui::Text("%lu", i);
                        ImGui::NextColumn();

                        ImGui::PushItemWidth(-1);
                        if(ImGui::DragFloat3("##Position",glm::value_ptr(pos)))
                        {
                           gc->setLightPosition(i, pos);
                        }
                        ImGui::PopItemWidth();
                        ImGui::NextColumn();

                        ImGui::PushItemWidth(-1);
                        if (ImGui::DragFloat3("##Color",glm::value_ptr(color)))
                        {
                            gc->setLightColor(i,color);
                        }
                        ImGui::PopItemWidth();
                        ImGui::NextColumn();
                        ImGui::PopID();
                    }
                }
            }
        }
        ImGui::Separator();
        ImGui::PopID();
    }

    void ToolPropertiesWindow::drawGridSettings()
    {
        ImGui::PushID("GridSettings");
        bool showGrid = mContext->getGrid()->getVisible();
        if (ImGui::Checkbox("Show Grid",&showGrid))
        {
            mContext->getGrid()->setVisible(showGrid);
        }

        ImGui::Separator();

        vec3 gridTx = mContext->getGrid()->getTransform().getTranslation();

        if(ImGui::DragFloat3("Position",glm::value_ptr(gridTx)),mContext->getGrid()->getMinorSpacing())
        {
            auto transform = mContext->getGrid()->getTransform();
            transform.setTranslation(gridTx);
            mContext->getGrid()->setTransform(transform);
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
        if(ImGui::DragFloat("Minor Spacing",&min,.1f))
        {
            mContext->getGrid()->setMinorSpacing(min);
            mContext->getGrid()->recalculateGridLines();
        }

        ImGui::Separator();

        vec4 majColorVec = mContext->getGrid()->getMajorColour();

        if (ImGui::ColorEdit4("Major Color",glm::value_ptr(majColorVec)))
        {
            mContext->getGrid()->setMajorColour(majColorVec);
            mContext->getGrid()->recalculateGridLines();
        }

        vec4 minColorVec = mContext->getGrid()->getMinorColour();

        if (ImGui::ColorEdit4("Minor Color",glm::value_ptr(minColorVec)))
        {
            mContext->getGrid()->setMinorColour(minColorVec);
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
        ImGui::PopID();
    }

    void ToolPropertiesWindow::draw()
    {
        if (mContext->getProject())
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
