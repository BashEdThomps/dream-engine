#pragma once

#include <DreamCore.h>
#include <OpenALAudioComponent.h>

#include "InputTarget.h"
#include "DreamToolWindow.h"

// ImGui Widgets
#include "Widgets/ImGui/ProjectBrowser.h"
#include "Widgets/ImGui/PropertiesWindow.h"
#include "Widgets/ImGui/MenuBar.h"
#include "Widgets/ImGui/ScriptDebugWindow.h"
#include "Widgets/ImGui/SceneStateWindow.h"
#include "Widgets/ImGui/ToolPropertiesWindow.h"
#include "Widgets/ImGui/RenderingDebugWindow.h"
#include "Widgets/ImGui/GamepadStateWindow.h"
#include "Widgets/ImGui/CacheContentWindow.h"
#include "Widgets/ImGui/TaskManagerWindow.h"
#include "Widgets/ImGui/GLPreviewWindowComponent.h"

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

using octronic::dream::ProjectRuntime;
using octronic::dream::SceneRuntime;
using octronic::dream::StorageManager;
using octronic::dream::open_al::OpenALAudioComponent;

namespace octronic::dream::tool
{
    class DreamToolContext
    {
    public:
        DreamToolContext(int _argc, char** _argv);
        ~DreamToolContext();

        bool init();
        void run();
        void handleEditorInput(SceneRuntime*);
        void handleSceneInput(SceneRuntime*);
        void closeProject();
        bool openProject(const string &);
        bool newProject(const string &dir);
        bool openInExternalEditor(AssetDefinition* definition, const string& format = "");

        // ImGui Widgets
        DreamToolWindow*          getWindow();
        octronic::dream::Project* getProject();
        PropertiesWindow*         getPropertiesWindow();
        ProjectBrowser*           getProjectBrowser();
        ScriptDebugWindow*        getScriptDebugWindow();
        SceneStateWindow*         getSceneStateWindow();
        ToolPropertiesWindow*     getToolPropertiesWindow();
        MenuBar*                  getMenuBar();
        RenderingDebugWindow*     getRenderPipelineWindow();
        GamepadStateWindow*       getGamepadStateWindow();
        CacheContentWindow*       getCacheContentWindow();
        TaskManagerWindow*        getTaskManagerWindow();
        GLPreviewWindowComponent* getGlPreviewWindowComponent();
        // GL Widgets
        Grid*                 getGrid();
        LightViewer*          getLightViewer();
        SelectionHighlighter* getSelectionHighlighter();
        Cursor*               getCursor();
        PathViewer*           getPathViewer();
        AnimationViewer*      getAnimationViewer();

        // Data Models
        TemplatesModel*               getTemplatesModel();
        PreferencesModel*             getPreferencesModel();
        ModelDefinitionBatchImporter* getModelDefinitionBatchImporter();
        ProjectDirectory*             getProjectDirectory();
        StorageManager*               getStorageManager();

        int         getCommandArgumentsCount();
        char**      getCommandArgumentsVector();
        bool        getMainLoopDone();
        void        setMainLoopDone(bool b);
        InputTarget getInputTarget();
        void        setInputTarget(InputTarget t);
        string      getLastDirectory();
        void        setLastDirectory(const string& lastDir);

    private:
        // ImGui Widgets
        DreamToolWindow mWindow;
        Project* mProject;
        PropertiesWindow mPropertiesWindow;
        ProjectBrowser mProjectBrowser;
        ScriptDebugWindow mScriptDebugWindow;
        SceneStateWindow mSceneStateWindow;
        ToolPropertiesWindow mToolPropertiesWindow;
        MenuBar mMenuBar;
        RenderingDebugWindow mRenderPipelineWindow;
        GamepadStateWindow mGamepadStateWindow;
        CacheContentWindow mCacheContentWindow;
        TaskManagerWindow mTaskManagerWindow;
        GLPreviewWindowComponent mGlPreviewWindowComponent;

        // GL Widgets
        Grid mGrid;
        LightViewer mLightViewer;
        SelectionHighlighter mSelectionHighlighter;
        Cursor mCursor;
        PathViewer mPathViewer;
        AnimationViewer mAnimationViewer;

        // Data Models
        TemplatesModel mTemplatesModel;
        PreferencesModel mPreferencesModel;
        ModelDefinitionBatchImporter mModelDefinitionBatchImporter;
        ProjectDirectory mProjectDirectory;
        StorageManager mStorageManager;
        OpenALAudioComponent mAudioComponent;

        int mCommandArgumentsCount;
        char** mCommandArgumentsVector;
        bool mMainLoopDone;
        InputTarget mInputTarget;
        string mLastDirectory;
        Directory* mProjectBaseDirectory;
    };
}
