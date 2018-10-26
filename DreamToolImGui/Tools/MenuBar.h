#pragma once

#include "DTWidget.h"

namespace Dream
{
    class Project;
}

using Dream::Project;

namespace DreamTool
{
    class MenuBar : public DTWidget
    {
    public:
        MenuBar(Project*);
        ~MenuBar() override;

        void draw() override;
    };
}
