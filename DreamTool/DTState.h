#pragma once

#ifdef WIN32
#include <GL/glew.h>
#endif
#include "../../DreamCore/Common/DreamObject.h"
#include "Window/DTWindowComponent.h"
// ImGui Widgets
#include "Widgets/ImGui/ProjectBrowser.h"
#include "Widgets/ImGui/PropertiesWindow.h"
#include "Widgets/ImGui/MenuBar.h"
#include "Widgets/ImGui/LuaDebugWindow.h"
#include "Widgets/ImGui/SceneStateWindow.h"
#include "Widgets/ImGui/GridPropertiesWindow.h"
#include "Widgets/ImGui/ScriptEditorWindow.h"
// Plain GL Widgets
#include "Widgets/GL/Grid.h"
#include "Widgets/GL/LightViewer.h"
#include "Widgets/GL/SelectionHighlighter.h"
#include "Widgets/GL/Cursor.h"
// Models
#include "Model/TemplatesModel.h"
#include "Model/PreferencesModel.h"

namespace Dream
{
    class Project;
}

namespace DreamTool
{
    class DTState : public DreamObject
    {
    public:
        // ImGui Widgets
        DTWindowComponent windowComponent;
        Dream::Project* project;
        PropertiesWindow propertiesWindow;
        ProjectBrowser projectBrowser;
        LuaDebugWindow luaDebugWindow;
        SceneStateWindow sceneStateWindow;
        GridPropertiesWindow gridPropertiesWindow;
        ScriptEditorWindow scriptEditorWindow;
        MenuBar menuBar;
        // GL Widgets
        Grid grid;
        LightViewer lightViewer;
        SelectionHighlighter selectionHighlighter;
        Cursor cursor;
        // Data Models
        TemplatesModel templatesModel;
        PreferencesModel preferencesModel;

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
        string lastDirectory;
    };
}
