#pragma once

#include "ImGuiWidget.h"

namespace Dream
{
    class Project;
}

using namespace Dream;

namespace DreamTool
{
    class LuaDebugWindow;
    class ProjectBrowser;
    class PropertiesWindow;
    class SceneStateWindow;

    class MenuBar : public ImGuiWidget
    {
    public:
        MenuBar(
            Project*,
            ProjectBrowser*,
            PropertiesWindow*,
            LuaDebugWindow*,
            SceneStateWindow*
        );
        ~MenuBar() override;

        void draw() override;
    private:
        ProjectBrowser* mProjectBrowser;
        PropertiesWindow* mPropertiesWindow;
        LuaDebugWindow* mLuaDebugWindow;
        SceneStateWindow* mSceneStateWindow;
    };
}