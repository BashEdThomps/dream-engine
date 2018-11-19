#pragma once

#include "ImGuiWidget.h"

namespace DreamTool
{
    class RenderPipelinePreviewWindow
            : public ImGuiWidget
    {
    public:
        RenderPipelinePreviewWindow(DTState* state);
        ~RenderPipelinePreviewWindow() override;

        void draw() override;
        static ImVec2 PreviewSize, UV1, UV2;
    };
}
