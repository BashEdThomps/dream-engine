#pragma once

#include "ImGuiWidget.h"

namespace octronic::dream::tool
{
    class RenderingDebugWindow
            : public ImGuiWidget
    {
    public:
        RenderingDebugWindow(DreamToolContext* state);
        ~RenderingDebugWindow() override;

        void draw() override;
        static ImVec2 PreviewSize, UV1, UV2;
    };
}
