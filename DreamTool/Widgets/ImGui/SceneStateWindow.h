#pragma once

#include "ImGuiWidget.h"

namespace DreamTool
{
    class SceneStateWindow
        : public ImGuiWidget
    {
    public:
        SceneStateWindow(Dream::Project*);
        ~SceneStateWindow() override;

        void draw() override;
    };
}
