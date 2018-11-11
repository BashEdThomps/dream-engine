#pragma once

#include "../../DreamCore/Common/DreamObject.h"
#include "Window/DTWindowComponent.h"
// ImGui Widgets
#include "Widgets/ImGui/ProjectBrowser.h"
#include "Widgets/ImGui/PropertiesWindow.h"
#include "Widgets/ImGui/MenuBar.h"
#include "Widgets/ImGui/LuaDebugWindow.h"
#include "Widgets/ImGui/SceneStateWindow.h"
#include "Widgets/ImGui/GridPropertiesWindow.h"
// Plain GL Widgets
#include "Widgets/GL/Grid.h"
#include "Widgets/GL/LightViewer.h"
#include "Widgets/GL/SelectionHighlighter.h"
#include "Widgets/GL/Cursor.h"

namespace Dream
{
    class Project;
}

namespace DreamTool
{
    class DTState : public DreamObject
    {
    public:
        // Widgets
        DTWindowComponent windowComponent;
        Dream::Project* project;
        PropertiesWindow propertiesWindow;
        ProjectBrowser projectBrowser;
        LuaDebugWindow luaDebugWindow;
        SceneStateWindow sceneStateWindow;
        GridPropertiesWindow gridPropertiesWindow;
        MenuBar menuBar;
        Grid grid;
        LightViewer lightViewer;
        SelectionHighlighter selectionHighlighter;
        Cursor cursor;
        int argc;
        char** argv;
        bool MainLoopDone = false;
        unsigned int Frames = 0;
        double CurrentTime = glfwGetTime();
        double OneSec = 1.0;
        bool CountFPS = false;

        DTState(int _argc, char** _argv);
        ~DTState();
        void init();
        void run();
        void showFPS();
    };
}
