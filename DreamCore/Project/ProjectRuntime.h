#pragma once

// Base
#include "Base/Runtime.h"
// Time
#include "Components/Time.h"
// Audio
#include "Components/Audio/AudioComponent.h"
#include "Components/Audio/AudioDefinition.h"
#include "Components/Audio/AudioRuntime.h"
// Input
#include "Components/Input/InputComponent.h"
// Graphics
#include "Components/Graphics/GraphicsComponent.h"
#include "Components/Graphics/Font/FontDefinition.h"
#include "Components/Graphics/Font/FontRuntime.h"
#include "Components/Graphics/Shader/ShaderDefinition.h"
#include "Components/Graphics/Shader/ShaderRuntime.h"
#include "Components/Graphics/Texture/TextureDefinition.h"
#include "Components/Graphics/Texture/TextureRuntime.h"
#include "Components/Graphics/Material/MaterialDefinition.h"
#include "Components/Graphics/Material/MaterialRuntime.h"
#include "Components/Graphics/Model/ModelDefinition.h"
#include "Components/Graphics/Model/ModelRuntime.h"
// Physics
#include "Components/Physics/PhysicsComponent.h"
// Script
#include "Components/Script/ScriptComponent.h"
#include "Components/Script/ScriptDefinition.h"
#include "Components/Script/ScriptRuntime.h"
// Task
#include "Task/Task.h"
#include "Task/TaskQueue.h"
// Cache
#include "Components/Cache.h"
// STD
#include <string>
#include <vector>
#include <deque>
#include <memory>
// Using
using std::string;
using std::vector;
using std::deque;

namespace octronic::dream
{
  // Typedef
  typedef Cache<AudioDefinition,AudioRuntime>       AudioCache;
  typedef Cache<FontDefinition,FontRuntime>         FontCache;
  typedef Cache<MaterialDefinition,MaterialRuntime> MaterialCache;
  typedef Cache<ModelDefinition,ModelRuntime>       ModelCache;
  typedef Cache<ScriptDefinition,ScriptRuntime>     ScriptCache;
  typedef Cache<ShaderDefinition,ShaderRuntime>     ShaderCache;
  typedef Cache<TextureDefinition,TextureRuntime>   TextureCache;
  // Forward Declarations
  class ProjectDirectory;
  class AssetDefinition;
  class WindowComponent;
  class StorageManager;
  class SceneRuntime;
  class EntityRuntime;
  class SceneDefinition;

  // Class Declaration
  class ProjectRuntime : public Runtime
  {
  public: // Public Variables
    static unsigned int MaxFrameCount;
  public: // Public Functions
    ProjectRuntime(ProjectDefinition& definition, ProjectDirectory& directory,
                   StorageManager& sm, WindowComponent& wc, AudioComponent& ac);

    ProjectRuntime(ProjectRuntime&&) = default;
    ProjectRuntime& operator=(ProjectRuntime&&)= default;


    void setDone(bool);
    bool isDone() const;

    ProjectDirectory& getProjectDirectory();
    // Components ==========================================================
    bool initComponents();
    Time&              getTime();
    AudioComponent&    getAudioComponent();
    PhysicsComponent&  getPhysicsComponent();
    GraphicsComponent& getGraphicsComponent();
    WindowComponent&   getWindowComponent();
    ScriptComponent&   getScriptComponent();
    InputComponent&    getInputComponent();
    StorageManager&    getStorageManager();
    TaskQueue<Task>&   getTaskQueue();
    TaskQueue<DestructionTask>& getDestructionTaskQueue();
    // Running =============================================================
    bool loadFromDefinition() override;
    void step();
    // Caches ==============================================================
    AudioCache&    getAudioCache();
    FontCache&     getFontCache();
    MaterialCache& getMaterialCache();
    ModelCache&    getModelCache();
    ScriptCache&   getScriptCache();
    ShaderCache&   getShaderCache();
    TextureCache&  getTextureCache();
    bool initCaches();
    void clearAllCaches();
    // Scenes ==============================================================
    SceneRuntime& createSceneRuntime(SceneDefinition&);
    optional<reference_wrapper<SceneRuntime>> getActiveSceneRuntime() const;
    optional<reference_wrapper<SceneRuntime>> getSceneRuntimeByUuid(UuidType uuid) const;
    bool hasActiveSceneRuntime() const;
    void removeSceneRuntime(const SceneRuntime&);
    void setActiveSceneRuntime(UuidType uuid);
    vector<reference_wrapper<SceneRuntime>> getSceneRuntimeVector();
    bool hasSceneRuntime(UuidType uuid) const;
    bool hasLoadedScenes() const;
    // Frames ==============================================================
    deque<float> getFrameDurationHistory() const;
    float getAverageFramerate();

  private: // Member Functions
    bool initAudioComponent();
    bool initInputComponent();
    bool initPhysicsComponent();
    bool initGraphicsComponent();
    bool initWindowComponent();
    bool initScriptComponent();
    void pushComponentTasks();
  private: // Member Variables
    bool mDone;
    reference_wrapper<ProjectDirectory> mProjectDirectory;
    reference_wrapper<AudioComponent>  mAudioComponent;
    reference_wrapper<WindowComponent> mWindowComponent;
    reference_wrapper<StorageManager> mStorageManager;
    // Project created Components
    Time              mTime;
    InputComponent    mInputComponent;
    GraphicsComponent mGraphicsComponent;
    PhysicsComponent  mPhysicsComponent;
    ScriptComponent   mScriptComponent;
    // Caches, Project owns those too
    AudioCache    mAudioCache;
    FontCache     mFontCache;
    MaterialCache mMaterialCache;
    ModelCache    mModelCache;
    ScriptCache   mScriptCache;
    ShaderCache   mShaderCache;
    TextureCache  mTextureCache;
    // SceneRuntime
    vector<unique_ptr<SceneRuntime>> mSceneRuntimeVector;
    vector<reference_wrapper<SceneRuntime>> mSceneRuntimesToRemove;
    optional<reference_wrapper<SceneRuntime>> mActiveSceneRuntime;
    // Tasking
    TaskQueue<Task>            mTaskQueue;
    TaskQueue<DestructionTask> mDestructionTaskQueue;
    // Frames
    deque<float> mFrameDurationHistory;
  };
}
