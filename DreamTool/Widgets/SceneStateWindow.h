#pragma once

#include "DTWidget.h"

namespace DreamTool
{
    class SceneStateWindow
        : public DTWidget
    {
    public:
        SceneStateWindow(Project*);
        ~SceneStateWindow() override;

        void draw() override;
    };
}
