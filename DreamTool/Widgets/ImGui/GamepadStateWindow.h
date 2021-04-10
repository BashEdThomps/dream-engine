#pragma once

#include "ImGuiWidget.h"

namespace octronic::dream::tool
{
    class GamepadStateWindow : public ImGuiWidget
    {
    public:
        GamepadStateWindow(DreamToolContext&);
        void draw() override;
    };
}
