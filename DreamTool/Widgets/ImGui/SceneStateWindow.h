#pragma once

#include "ImGuiWidget.h"

namespace DreamTool
{
    class SceneStateWindow
        : public ImGuiWidget
    {
    public:
        SceneStateWindow(Project*);
        ~SceneStateWindow() override;

        void draw() override;
    };
}
