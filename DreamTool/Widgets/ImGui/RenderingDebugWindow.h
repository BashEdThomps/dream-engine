#pragma once

#include "ImGuiWidget.h"

namespace DreamTool
{
    class RenderingDebugWindow
            : public ImGuiWidget
    {
    public:
        RenderingDebugWindow(DTState* state);
        ~RenderingDebugWindow() override;

        void draw() override;
        static ImVec2 PreviewSize, UV1, UV2;
    };
}
