#pragma once

#include "ImGuiWidget.h"

namespace DreamTool
{
    class GamepadStateWindow : public ImGuiWidget
    {
    public:
        GamepadStateWindow(DTState*, bool visible = false);
        ~GamepadStateWindow() override;
        void draw() override;
    };
}
