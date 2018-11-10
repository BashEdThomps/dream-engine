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
    class GridPropertiesWindow;
	class SelectionHighlighterWidget;

    class MenuBar : public ImGuiWidget
    {
    public:
        MenuBar(
            Dream::Project*,
            ProjectBrowser*,
            PropertiesWindow*,
            LuaDebugWindow*,
            SceneStateWindow*,
            GridPropertiesWindow*
        );
        ~MenuBar() override;
        void draw() override;
		void setSelectionHighlighter(SelectionHighlighterWidget* sh);
    private:
        ProjectBrowser* mProjectBrowser;
        PropertiesWindow* mPropertiesWindow;
        LuaDebugWindow* mLuaDebugWindow;
        SceneStateWindow* mSceneStateWindow;
        GridPropertiesWindow* mGridPropertiesWindow;
		SelectionHighlighterWidget* mSelectionHighlighter;
    };
}
