#pragma once

#ifdef WIN32
#include <GL/glew.h>
#endif
#include "../DreamCore/Common/DreamObject.h"
#include "Window/DTWindowComponent.h"
// ImGui Widgets
#include "Widgets/ImGui/ProjectBrowser.h"
#include "Widgets/ImGui/PropertiesWindow.h"
#include "Widgets/ImGui/MenuBar.h"
#include "Widgets/ImGui/LuaDebugWindow.h"
#include "Widgets/ImGui/SceneStateWindow.h"
#include "Widgets/ImGui/GridPropertiesWindow.h"
#include "Widgets/ImGui/ScriptEditorWindow.h"
#include "Widgets/ImGui/ShaderEditorWindow.h"
#include "Widgets/ImGui/RenderPipelinePreviewWindow.h"
#include "Widgets/ImGui/GamepadStateWindow.h"
// Plain GL Widgets
#include "Widgets/GL/Grid.h"
#include "Widgets/GL/LightViewer.h"
#include "Widgets/GL/SelectionHighlighter.h"
#include "Widgets/GL/Cursor.h"
// Models
#include "Model/TemplatesModel.h"
#include "Model/PreferencesModel.h"
#include "Model/ModelDefinitionBatchImporter.h"

namespace Dream
{
    class Project;
    class SceneRuntime;
}

using namespace Dream;

namespace DreamTool
{
    class DTState : public DreamObject
    {
    public:
        enum InputTarget
        {
            EDITOR,
            SCENE
        };

        // ImGui Widgets
        DTWindowComponent windowComponent;
        Dream::Project* project;
        PropertiesWindow propertiesWindow;
        ProjectBrowser projectBrowser;
        LuaDebugWindow luaDebugWindow;
        SceneStateWindow sceneStateWindow;
        GridPropertiesWindow gridPropertiesWindow;
        ScriptEditorWindow scriptEditorWindow;
        ShaderEditorWindow shaderEditorWindow;
        MenuBar menuBar;
        RenderPipelinePreviewWindow renderPipelineWindow;
        GamepadStateWindow gamepadStateWindow;
        // GL Widgets
        Grid grid;
        LightViewer lightViewer;
        SelectionHighlighter selectionHighlighter;
        Cursor cursor;
        // Data Models
        TemplatesModel templatesModel;
        PreferencesModel preferencesModel;
        ModelDefinitionBatchImporter modelDefinitionBatchImporter;
        ProjectDirectory projectDirectory;

        int argc;
        char** argv;
        bool MainLoopDone = false;
        double LastTime = glfwGetTime();
        double CurrentTime = glfwGetTime();
        double Frames = 0;
        InputTarget inputTarget=EDITOR;
        string lastDirectory;

        DTState(int _argc, char** _argv);
        ~DTState();

        void init();
        void run();
        void FPS();
        void handleEditorInput(SceneRuntime*);
        void handleSceneInput(SceneRuntime*);
        void closeProject();
        bool openProject(string);
        bool newProject(string dir);
    };
}
