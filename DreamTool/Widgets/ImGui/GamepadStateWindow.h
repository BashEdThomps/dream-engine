#pragma once

#include "ImGuiWidget.h"

namespace octronic::dream::tool
{
    class GamepadStateWindow : public ImGuiWidget
    {
    public:
        GamepadStateWindow(DreamToolContext&, bool visible = false);
        ~GamepadStateWindow() override;
        void draw() override;
    };
}
