#pragma once

#include "ImGuiWidget.h"

namespace octronic::dream::tool
{
    class SceneStateWindow
        : public ImGuiWidget
    {
    public:
        SceneStateWindow(DreamToolContext&);
        ~SceneStateWindow() override;

        void draw() override;
    };
}
