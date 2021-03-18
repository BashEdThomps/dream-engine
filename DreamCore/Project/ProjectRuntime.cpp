/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#include "ProjectRuntime.h"

#include "Project.h"
#include "ProjectDefinition.h"
#include "Storage/ProjectDirectory.h"

#include "Common/Logger.h"

#include "Scene/SceneRuntime.h"
#include "Scene/SceneDefinition.h"
#include "Entity/EntityRuntime.h"

#include "Components/AssetDefinition.h"
#include "Components/Time.h"
#include "Math/Transform.h"
#include "Components/Cache.h"

#include "Components/Audio/AudioComponent.h"
#include "Components/Audio/AudioRuntime.h"

#include "Components/Graphics/Model/ModelMesh.h"
#include "Components/Window/WindowComponent.h"

using std::make_shared;
using std::make_unique;
using std::static_pointer_cast;

namespace octronic::dream
{
  ProjectRuntime::ProjectRuntime
  (const weak_ptr<ProjectDefinition>& project,
   const weak_ptr<ProjectDirectory>& directory,
   const shared_ptr<WindowComponent>& windowComponent,
   const shared_ptr<AudioComponent>& ac,
   const shared_ptr<StorageManager>& fm)
    : Runtime(project),
      mProjectDirectory(directory),
      mDone(false),
      mAudioComponent(ac),
      mWindowComponent(windowComponent),
      mStorageManager(fm),
      mTime(make_shared<Time>()),
      mTaskQueue(make_shared<TaskQueue<Task>>("ProjectTaskQueue")),
      mDestructionTaskQueue(make_shared<TaskQueue<DestructionTask>>("ProjectDestructionTaskQueue"))
  {
    LOG_DEBUG( "ProjectRuntime: Constructing" );
    mFrameDurationHistory.resize(MaxFrameCount);
  }

  ProjectRuntime::~ProjectRuntime
  ()
  {
    LOG_DEBUG( "ProjectRuntime: Destructing" );
  }

  // Init ====================================================================

  bool
  ProjectRuntime::initComponents
  ()
  {
    LOG_TRACE( "ProjectRuntime: {}",__FUNCTION__ );

    if (!initWindowComponent())
    {
      LOG_ERROR("ProjectRuntime: Failed to init WindowComponent");
      return false;
    }

    if (!initGraphicsComponent())
    {
      LOG_ERROR("ProjectRuntime: Failed to init GraphicsComponent");
      return false;
    }

    if (!initInputComponent())
    {
      LOG_ERROR("ProjectRuntime: Failed to init InputComponent");
      return false;
    }

    if(!initPhysicsComponent())
    {
      LOG_ERROR("ProjectRuntime: Failed to init PhysicsComponent");
      return false;
    }

    if(!initAudioComponent())
    {
      LOG_ERROR("ProjectRuntime: Failed to init AudioComponent");
      return false;
    }

    if (!initScriptComponent())
    {
      LOG_ERROR("ProjectRuntime: Failed to init ScriptComponent");
      return false;
    }

    LOG_DEBUG( "ProjectRuntime: Successfully created Components." );

    return true;
  }

  bool
  ProjectRuntime::initWindowComponent
  ()
  {
    LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
    if (!mWindowComponent)
    {
      LOG_CRITICAL("ProjectRuntime: Window component is null");
      return false;
    }
    auto projDef = static_pointer_cast<ProjectDefinition>(mDefinition.lock());
    mWindowComponent->setName(projDef->getName());
    return true;
  }

  bool
  ProjectRuntime::initAudioComponent
  ()
  {
    LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
    // Audio component must be initialised outside of dream
    if(mAudioComponent == nullptr)
    {
      LOG_ERROR("ProjectRuntime: AudioComponent is null");
      return false;
    }
    mAudioComponent->setProjectRuntime(static_pointer_cast<ProjectRuntime>(shared_from_this()));
    return true;
  }

  bool ProjectRuntime::initInputComponent()
  {
    LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);

    mInputComponent = make_shared<InputComponent>(static_pointer_cast<ProjectRuntime>(shared_from_this()));
    if (!mInputComponent->init())
    {
      LOG_ERROR( "ProjectRuntime: Unable to initialise InputComponent." );
      return false;
    }
    return true;
  }

  bool
  ProjectRuntime::initPhysicsComponent
  ()
  {
    LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
    mPhysicsComponent = make_shared<PhysicsComponent>(static_pointer_cast<ProjectRuntime>(shared_from_this()));

    if (!mPhysicsComponent->init())
    {
      LOG_ERROR( "ProjectRuntime: Unable to initialise PhysicsComponent." );
      return false;
    }
    return true;
  }

  bool
  ProjectRuntime::initGraphicsComponent
  ()
  {
    LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);

    mGraphicsComponent = make_shared<GraphicsComponent>(static_pointer_cast<ProjectRuntime>(shared_from_this()));
    if (!mGraphicsComponent->init())
    {
      LOG_ERROR( "ProjectRuntime: Unable to initialise Graphics Component." );
      return false;
    }
    return true;
  }

  bool
  ProjectRuntime::initScriptComponent
  ()
  {
    LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);

    mScriptComponent = make_shared<ScriptComponent>(static_pointer_cast<ProjectRuntime>(shared_from_this()));
    if(!mScriptComponent->init())
    {
      LOG_ERROR( "ProjectRuntime: Unable to initialise Script Engine." );
      return false;
    }
    return true;
  }

  bool
  ProjectRuntime::constructSceneRuntime
  (const weak_ptr<SceneRuntime>& rt)
  {
    LOG_DEBUG("ProjectRuntime: Constructing Scene Runtime");
    if (auto srLock = rt.lock())
    {
      if (auto task = srLock->getLoadFromDefinitionTask().lock())
      {
        if (task->hasState(TASK_STATE_QUEUED))
        {
          mTaskQueue->pushTask(task);
          return true;
        }
      }
    }
    return false;
  }

  bool
  ProjectRuntime::loadFromDefinition
  ()
  {
    LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);

    if (!initCaches())
    {
      return false;
    }

    if (!initComponents())
    {
      return false;
    }

    return true;
  }

  bool ProjectRuntime::initCaches()
  {
    mAudioCache    =  make_shared<AudioCache>(static_pointer_cast<ProjectRuntime>(shared_from_this()));
    mTextureCache  =  make_shared<TextureCache>(static_pointer_cast<ProjectRuntime>(shared_from_this()));
    mMaterialCache =  make_shared<MaterialCache>(static_pointer_cast<ProjectRuntime>(shared_from_this()));
    mModelCache    =  make_shared<ModelCache>(static_pointer_cast<ProjectRuntime>(shared_from_this()));
    mShaderCache   =  make_shared<ShaderCache>(static_pointer_cast<ProjectRuntime>(shared_from_this()));
    mScriptCache   =  make_shared<ScriptCache>(static_pointer_cast<ProjectRuntime>(shared_from_this()));
    mFontCache     =  make_shared<FontCache>(static_pointer_cast<ProjectRuntime>(shared_from_this()));
    return true;
  }

  void
  ProjectRuntime::destructSceneRuntime
  (const weak_ptr<SceneRuntime>& rt)
  {
    LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
    if (auto srLock = rt.lock())
    {
      srLock->destroyRuntime();
      if (mActiveSceneRuntime.lock().get() == srLock.get())
      {
        mActiveSceneRuntime = weak_ptr<SceneRuntime>();
      }
    }
  }

  // Running =================================================================

  void
  ProjectRuntime::collectGarbage
  ()
  {
    LOG_DEBUG("\n"
        "=======================================================================\n"
        "CollectGarbage Called @ {}\n"
        "=======================================================================",
              mTime->getAbsoluteTime());

    mDestructionTaskQueue->executeQueue();
    if (auto gfxDestQueue = mGraphicsComponent->getDestructionTaskQueue().lock())
    {
      gfxDestQueue->executeQueue();
    }

    LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
    for (auto sceneRuntimeWeak : mSceneRuntimesToRemove)
    {
      if (auto srLock = sceneRuntimeWeak.lock())
      {
        srLock->collectGarbage();
        auto itr = find(mSceneRuntimeVector.begin(),mSceneRuntimeVector.end(),srLock);
        if (itr != mSceneRuntimeVector.end())
        {
          mSceneRuntimeVector.erase(itr);
        }
      }
    }
    mSceneRuntimesToRemove.clear();
  }

  void ProjectRuntime::pushComponentTasks()
  {
    // Maintain this order
    mWindowComponent->pushTasks();
    mInputComponent->pushTasks();
    mAudioComponent->pushTasks();
    mPhysicsComponent->pushTasks();
    mScriptComponent->pushTasks();
    mGraphicsComponent->pushTasks();
  }

  void
  ProjectRuntime::step
  ()
  {
    LOG_TRACE("\n\n=========================[ Update Started ]=========================\n\n");

    pushComponentTasks();

    for (auto rt : mSceneRuntimeVector)
    {
      switch (rt->getState())
      {
        case SceneState::SCENE_STATE_TO_LOAD:
          constructSceneRuntime(rt);
          break;
        case SceneState::SCENE_STATE_LOADED:
          break;
        case SceneState::SCENE_STATE_ACTIVE:
          mTime->updateFrameTime();
          mFrameDurationHistory.push_back(1000.0f/mTime->getFrameTimeDelta());
          if (mFrameDurationHistory.size() > MaxFrameCount) mFrameDurationHistory.pop_front();

          if (auto camera = rt->getCamera().lock())
          {
            camera->update();
          }
          rt->updateFlatVector();
          rt->createSceneTasks();

          rt->collectGarbage();
          break;
        case SceneState::SCENE_STATE_TO_DESTROY:
          destructSceneRuntime(rt);
          break;
        case SceneState::SCENE_STATE_DESTROYED:
          mSceneRuntimesToRemove.push_back(rt);
          break;
      }
    }

    ModelMesh::ClearCounters();
    ShaderRuntime::InvalidateState();
    mTaskQueue->executeQueue();
    if (auto gfxQueue = mGraphicsComponent->getTaskQueue().lock())
    {
      gfxQueue->executeQueue();
    }

    collectGarbage();

    LOG_TRACE("\n\n=========================[ Update Complete ]=========================\n\n");
  }

  void
  ProjectRuntime::clearAllCaches
  ()
  {
    LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);

    mDestructionTaskQueue->executeQueue();
    if (auto gfxDestQueue = mGraphicsComponent->getDestructionTaskQueue().lock())
    {
      gfxDestQueue->executeQueue();
    }

    mAudioCache->clear();
    mModelCache->clear();
    mShaderCache->clear();
    mMaterialCache->clear();
    mTextureCache->clear();
    mScriptCache->clear();
    mFontCache->clear();
  }

  // Accessors  ==============================================================

  weak_ptr<AudioCache>
  ProjectRuntime::getAudioCache
  ()
  const
  {
    return mAudioCache;
  }

  weak_ptr<ShaderCache>
  ProjectRuntime::getShaderCache
  ()
  const
  {
    return mShaderCache;
  }

  weak_ptr<MaterialCache>
  ProjectRuntime::getMaterialCache
  ()
  const
  {
    return mMaterialCache;
  }

  weak_ptr<ModelCache>
  ProjectRuntime::getModelCache
  ()
  const
  {
    return mModelCache;
  }

  weak_ptr<TextureCache>
  ProjectRuntime::getTextureCache
  ()
  const
  {
    return mTextureCache;
  }

  weak_ptr<ScriptCache>
  ProjectRuntime::getScriptCache
  ()
  const
  {
    return mScriptCache;
  }

  weak_ptr<FontCache>
  ProjectRuntime::getFontCache
  ()
  const
  {
    return mFontCache;
  }

  weak_ptr<AssetDefinition>
  ProjectRuntime::getAssetDefinitionByUuid
  (UuidType uuid)
  const
  {
    if (auto pdLock = static_pointer_cast<ProjectDefinition>(mDefinition.lock()))
    {
      return pdLock->getAssetDefinitionByUuid(uuid);
    }
    return weak_ptr<AssetDefinition>();
  }

  bool
  ProjectRuntime::hasActiveScene
  ()
  const
  {
    return mActiveSceneRuntime.expired();
  }

  bool
  ProjectRuntime::hasLoadedScenes
  ()
  const
  {
    for (auto srt : mSceneRuntimeVector)
    {
      if (srt->getState() >= SceneState::SCENE_STATE_LOADED &&
          srt->getState() < SceneState::SCENE_STATE_DESTROYED)
      {
        return true;
      }
    }
    return false;
  }


  bool
  ProjectRuntime::hasSceneRuntime
  (UuidType uuid)
  const
  {
    bool result = false;
    for (auto srt : mSceneRuntimeVector)
    {
      if (srt->getUuid() == uuid)
      {
        result = true;
      }

      if (result) break;
    }
    return result;
  }

  deque<float> ProjectRuntime::getFrameDurationHistory() const
  {
    return mFrameDurationHistory;
  }

  float ProjectRuntime::getAverageFramerate()
  {
    float f = 0.0;
    for (const auto& dur : mFrameDurationHistory)
    {
      f += dur;
    }
    return f/MaxFrameCount;
  }

  weak_ptr<TaskQueue<Task>>
  ProjectRuntime::getTaskQueue
  ()
  const
  {
    return mTaskQueue;
  }

  weak_ptr<TaskQueue<DestructionTask>>
  ProjectRuntime::getDestructionTaskQueue
  ()
  const
  {
    return mDestructionTaskQueue;
  }

  weak_ptr<WindowComponent>
  ProjectRuntime::getWindowComponent
  ()
  const
  {
    return mWindowComponent;
  }

  void
  ProjectRuntime::setDone
  (bool done)
  {
    mDone = done;
  }

  weak_ptr<Time>
  ProjectRuntime::getTime
  ()
  const
  {
    return mTime;
  }

  bool
  ProjectRuntime::isDone
  ()
  const
  {
    return mDone;
  }

  weak_ptr<AudioComponent>
  ProjectRuntime::getAudioComponent
  ()
  const
  {
    return mAudioComponent;
  }

  weak_ptr<PhysicsComponent>
  ProjectRuntime::getPhysicsComponent
  ()
  const
  {
    return mPhysicsComponent;
  }

  weak_ptr<GraphicsComponent>
  ProjectRuntime::getGraphicsComponent
  ()
  const
  {
    return mGraphicsComponent;
  }

  weak_ptr<ScriptComponent>
  ProjectRuntime::getScriptComponent
  ()
  const
  {
    return mScriptComponent;
  }

  weak_ptr<StorageManager>
  ProjectRuntime::getStorageManager
  ()
  const
  {
    return mStorageManager;
  }

  weak_ptr<SceneRuntime>
  ProjectRuntime::getActiveSceneRuntime
  ()
  const
  {
    LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
    return mActiveSceneRuntime;
  }

  weak_ptr<SceneRuntime>
  ProjectRuntime::getSceneRuntimeByUuid
  (UuidType uuid)
  const
  {
    LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
    for (auto sr : mSceneRuntimeVector)
    {
      if (sr->getUuid() == uuid)
      {
        return sr;
      }
    }
    return weak_ptr<SceneRuntime>();
  }

  void
  ProjectRuntime::setActiveSceneRuntime
  (UuidType uuid)
  {
    LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
    mActiveSceneRuntime = getSceneRuntimeByUuid(uuid);
    if (auto asrLock = mActiveSceneRuntime.lock())
    {
      asrLock->setState(SCENE_STATE_ACTIVE);
    }
  }

  vector<weak_ptr<SceneRuntime>>
  ProjectRuntime::getSceneRuntimeVector
  ()
  const
  {
    vector<weak_ptr<SceneRuntime>> ret;
    ret.insert(ret.end(), mSceneRuntimeVector.begin(), mSceneRuntimeVector.end());
    return ret;
  }

  void
  ProjectRuntime::addSceneRuntime
  (const shared_ptr<SceneRuntime>& rt)
  {
    auto itr = find(mSceneRuntimeVector.begin(), mSceneRuntimeVector.end(), rt);
    if (itr == mSceneRuntimeVector.end())
    {
      LOG_TRACE("ProjectRuntime: Adding SceneRuntime {}",rt->getNameAndUuidString());
      mSceneRuntimeVector.push_back(rt);
    }
  }

  void
  ProjectRuntime::removeSceneRuntime
  (const weak_ptr<SceneRuntime>& rt)
  {
    if (auto rtLock = rt.lock())
    {
    auto itr = find(mSceneRuntimeVector.begin(), mSceneRuntimeVector.end(), rtLock);
    if (itr != mSceneRuntimeVector.end())
    {
      mSceneRuntimeVector.erase(itr);
    }
    }
  }

  weak_ptr<InputComponent>
  ProjectRuntime::getInputComponent
  ()
  const
  {
    return mInputComponent;
  }

  weak_ptr<ProjectDirectory>
  ProjectRuntime::getProjectDirectory()
  const
  {
    return mProjectDirectory;
  }

  int ProjectRuntime::MaxFrameCount = 100;
}
