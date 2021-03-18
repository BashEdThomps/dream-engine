#pragma once

#include <DreamCore.h>
#include <OpenALAudioComponent.h>

#include "InputTarget.h"
#include "DreamToolWindow.h"

// ImGui Widgets
#include "Widgets/ImGui/ProjectBrowser.h"
#include "Widgets/ImGui/AssetBrowser.h"
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
#include "Widgets/GL/PhysicsDebugDrawer.h"

// Models
#include "Model/TemplatesModel.h"
#include "Model/PreferencesModel.h"
#include "Model/ModelDefinitionBatchImporter.h"

using octronic::dream::Project;
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
        void handleEditorInput(const shared_ptr<SceneRuntime>&);
        void handleSceneInput(const shared_ptr<SceneRuntime>&);
        void closeProject();
        bool openProject(const string&);
        bool newProject(const string& dir);
        bool openInExternalEditor(const shared_ptr<AssetDefinition>& definition, const string& format = "");

        // ImGui Widgets
        DreamToolWindow*          getWindow();
        shared_ptr<Project>       getProject() const;
        PropertiesWindow*         getPropertiesWindow();
        ProjectBrowser*           getProjectBrowser();
        AssetBrowser*             getAssetBrowser();
        SceneStateWindow*         getSceneStateWindow();
        ToolPropertiesWindow*     getToolPropertiesWindow();
        MenuBar*                  getMenuBar();
        RenderingDebugWindow*     getRenderPipelineWindow();
        GamepadStateWindow*       getGamepadStateWindow();
        CacheContentWindow*       getCacheContentWindow();
        TaskManagerWindow*        getTaskManagerWindow();
        shared_ptr<ScriptDebugWindow> getScriptDebugWindow() const;
        shared_ptr<GLPreviewWindowComponent> getGlPreviewWindowComponent() const;

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
        shared_ptr<ProjectDirectory>  getProjectDirectory() const;
        shared_ptr<StorageManager>    getStorageManager() const;

        int         getCommandArgumentsCount();
        char**      getCommandArgumentsVector();
        bool        getMainLoopDone();
        void        setMainLoopDone(bool b);
        InputTarget getInputTarget();
        void        setInputTarget(InputTarget t);
        string      getLastDirectory();
        void        setLastDirectory(const string& lastDir);

    private:
        shared_ptr<Project> mProject;

        // ImGui Widgets
        DreamToolWindow mWindow;
        PropertiesWindow mPropertiesWindow;
        ProjectBrowser mProjectBrowser;
        AssetBrowser mAssetBrowser;
        SceneStateWindow mSceneStateWindow;
        ToolPropertiesWindow mToolPropertiesWindow;
        MenuBar mMenuBar;
        RenderingDebugWindow mRenderPipelineWindow;
        GamepadStateWindow mGamepadStateWindow;
        CacheContentWindow mCacheContentWindow;
        TaskManagerWindow mTaskManagerWindow;

        shared_ptr<ScriptDebugWindow> mScriptDebugWindow;
        shared_ptr<GLPreviewWindowComponent> mGlPreviewWindowComponent;

        // GL Widgets
        Grid mGrid;
        LightViewer mLightViewer;
        SelectionHighlighter mSelectionHighlighter;
        Cursor mCursor;
        PathViewer mPathViewer;
        AnimationViewer mAnimationViewer;
        shared_ptr<PhysicsDebugDrawer> mPhysicsDebugDrawer;

        // Data Models
        TemplatesModel mTemplatesModel;
        PreferencesModel mPreferencesModel;
        ModelDefinitionBatchImporter mModelDefinitionBatchImporter;
        shared_ptr<ProjectDirectory> mProjectDirectory;
        shared_ptr<StorageManager> mStorageManager;
        shared_ptr<OpenALAudioComponent> mAudioComponent;

        int mCommandArgumentsCount;
        char** mCommandArgumentsVector;
        bool mMainLoopDone;
        InputTarget mInputTarget;
        string mLastDirectory;
        shared_ptr<Directory> mProjectBaseDirectory;
    };
}
