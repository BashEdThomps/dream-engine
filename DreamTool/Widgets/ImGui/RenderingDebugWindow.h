#pragma once

#include "ImGuiWidget.h"

namespace octronic::dream::tool
{
    class RenderingDebugWindow
            : public ImGuiWidget
    {
    public:
        RenderingDebugWindow(DreamToolContext& state);

        void draw() override;
        static ImVec2 UV1, UV2;
    };
}
