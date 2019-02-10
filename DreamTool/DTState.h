#pragma once

#ifdef WIN32
    #include <Windows.h>
    #include <GL/glew.h>
#endif

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include <GL/glew.h>
    #include <OpenGL/gl3.h>
#endif

#ifdef __linux__
    #include <GL/glew.h>
    #include <GL/gl.h>
#endif

#include "../DreamCore/Common/DreamObject.h"
#include "Window/DTWindowComponent.h"
// ImGui Widgets
#include "Widgets/ImGui/ProjectBrowser.h"
#include "Widgets/ImGui/PropertiesWindow.h"
#include "Widgets/ImGui/MenuBar.h"
#include "Widgets/ImGui/ScriptDebugWindow.h"
#include "Widgets/ImGui/SceneStateWindow.h"
#include "Widgets/ImGui/ToolPropertiesWindow.h"
#include "Widgets/ImGui/ScriptEditorWindow.h"
#include "Widgets/ImGui/ShaderEditorWindow.h"
#include "Widgets/ImGui/RenderingDebugWindow.h"
#include "Widgets/ImGui/GamepadStateWindow.h"
#include "Widgets/ImGui/CacheContentWindow.h"
#include "Widgets/ImGui/TaskManagerWindow.h"
// Plain GL Widgets
#include "Widgets/GL/Grid.h"
#include "Widgets/GL/LightViewer.h"
#include "Widgets/GL/SelectionHighlighter.h"
#include "Widgets/GL/Cursor.h"
#include "Widgets/GL/PathViewer.h"
#include "Widgets/GL/AnimationViewer.h"
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
        ScriptDebugWindow scriptDebugWindow;
        SceneStateWindow sceneStateWindow;
        ToolPropertiesWindow gridPropertiesWindow;
        ScriptEditorWindow scriptEditorWindow;
        ShaderEditorWindow shaderEditorWindow;
        MenuBar menuBar;
        RenderingDebugWindow renderPipelineWindow;
        GamepadStateWindow gamepadStateWindow;
        CacheContentWindow cacheContentWindow;
        TaskManagerWindow taskManagerWindow;
        // GL Widgets
        Grid grid;
        LightViewer lightViewer;
        SelectionHighlighter selectionHighlighter;
        Cursor cursor;
        PathViewer pathViewer;
        AnimationViewer animationViewer;
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

        bool init();
        void run();
        void FPS();
        void handleEditorInput(SceneRuntime*);
        void handleSceneInput(SceneRuntime*);
        void closeProject();
        bool openProject(const string &);
        bool newProject(const string &dir);
    };
}
