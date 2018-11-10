#pragma once

#include "ImGuiWidget.h"

namespace DreamTool
{
    class Grid;
    class GridPropertiesWindow
            : public ImGuiWidget
    {
    public:
        GridPropertiesWindow(Dream::Project* p);
        ~GridPropertiesWindow() override;
        void draw() override;
        void setGrid(Grid* gridHandle);

    protected:
        Grid* mGridHandle;
    };
}
