#pragma once

#include "DTWidget.h"

namespace Dream
{
    class Project;
}

using Dream::Project;

namespace DreamTool
{
    class LuaDebugWindow;
    class ProjectBrowser;
    class PropertiesWindow;

    class MenuBar : public DTWidget
    {
    public:
        MenuBar(Project*, ProjectBrowser*, PropertiesWindow*, LuaDebugWindow*);
        ~MenuBar() override;

        void draw() override;
    private:
        ProjectBrowser* mProjectBrowser;
        PropertiesWindow* mPropertiesWindow;
        LuaDebugWindow* mLuaDebugWindow;
    };
}
