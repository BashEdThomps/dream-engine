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

#include "ProjectDefinition.h"
#include "ProjectDirectory.h"

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
  (ProjectDefinition& project,
   ProjectDirectory& directory,
   StorageManager& sm,
   WindowComponent& windowComponent,
   AudioComponent& ac)
    : Runtime(project),
      mDone(false),
      mProjectDirectory(directory),
      mAudioComponent(ac),
      mWindowComponent(windowComponent),
      mStorageManager(sm),
      mInputComponent(*this),
      mGraphicsComponent(*this),
      mPhysicsComponent(*this),
      mScriptComponent(*this),
      mAudioCache(*this, static_cast<ProjectDefinition&>(getDefinition()),mProjectDirectory),
      mFontCache(*this, static_cast<ProjectDefinition&>(getDefinition()),mProjectDirectory),
      mMaterialCache(*this, static_cast<ProjectDefinition&>(getDefinition()),mProjectDirectory),
      mModelCache(*this, static_cast<ProjectDefinition&>(getDefinition()),mProjectDirectory),
      mScriptCache(*this, static_cast<ProjectDefinition&>(getDefinition()),mProjectDirectory),
      mShaderCache(*this, static_cast<ProjectDefinition&>(getDefinition()),mProjectDirectory),
      mTextureCache(*this, static_cast<ProjectDefinition&>(getDefinition()),mProjectDirectory),
      mTaskQueue("ProjectTaskQueue"),
      mDestructionTaskQueue("ProjectDestructionTaskQueue")
  {
    LOG_DEBUG( "ProjectRuntime: Constructing" );
    mFrameDurationHistory.resize(MaxFrameCount);
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
    auto& projDef = static_cast<ProjectDefinition&>(getDefinition());
    auto& wComp = mWindowComponent.get();
    wComp.setProjectRuntime(*this);
    wComp.setName(projDef.getName());
    return true;
  }

  bool
  ProjectRuntime::initAudioComponent
  ()
  {
    LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
    mAudioComponent.get().setProjectRuntime(*this);
    return true;
  }

  bool ProjectRuntime::initInputComponent()
  {
    LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);

    if (!mInputComponent.init())
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

    if (!mPhysicsComponent.init())
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

    if (!mGraphicsComponent.init())
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

    if(!mScriptComponent.init())
    {
      LOG_ERROR( "ProjectRuntime: Unable to initialise Script Engine." );
      return false;
    }
    return true;
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
    return true;
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
              mTime.getAbsoluteTime());

    mDestructionTaskQueue.executeQueue();
    auto& gfxDestQueue = mGraphicsComponent.getDestructionTaskQueue();
    gfxDestQueue.executeQueue();

    LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
    for (auto& sceneRuntime : mSceneRuntimesToRemove)
    {
      sceneRuntime.get().collectGarbage();
      removeSceneRuntime(sceneRuntime);
    }
    mSceneRuntimesToRemove.clear();
  }

  void ProjectRuntime::pushComponentTasks()
  {
    // Maintain this order
    mWindowComponent.get().pushTasks();
    mInputComponent.pushTasks();
    mAudioComponent.get().pushTasks();
    mPhysicsComponent.pushTasks();
    mScriptComponent.pushTasks();
    mGraphicsComponent.pushTasks();
  }

  void
  ProjectRuntime::step
  ()
  {
    LOG_TRACE("\n\n=========================[ Update Started ]=========================\n\n");

    pushComponentTasks();

    for (auto& rt_ptr : mSceneRuntimeVector)
    {
      auto& rt = *rt_ptr;
      switch (rt.getState())
      {
        case SceneState::SCENE_STATE_TO_LOAD:
        {
          if (auto task = rt.getLoadFromDefinitionTask())
          {
            if (task->hasState(TASK_STATE_QUEUED))
            {
              mTaskQueue.pushTask(task);
            }
          }
          break;
        }
        case SceneState::SCENE_STATE_LOADED:
        {
          break;
        }
        case SceneState::SCENE_STATE_ACTIVE:
        {
          mTime.updateFrameTime();
          mFrameDurationHistory.push_back(1000.0f/mTime.getFrameTimeDelta());
          if (mFrameDurationHistory.size() > MaxFrameCount) mFrameDurationHistory.pop_front();

          auto& camera = rt.getCameraRuntime();
          camera.update();
          rt.updateFlatVector();
          rt.createSceneTasks();

          rt.collectGarbage();
          break;
        }
        case SceneState::SCENE_STATE_TO_DESTROY:
        {
          rt.destroyRuntime();
          if (mActiveSceneRuntime.value().get() == rt)
          {
            mActiveSceneRuntime.reset();
          }
          break;
        }
        case SceneState::SCENE_STATE_DESTROYED:
        {
          mSceneRuntimesToRemove.push_back(rt);
          break;
        }
      }
    }

    ModelMesh::ClearCounters();
    ShaderRuntime::InvalidateState();

    mTaskQueue.executeQueue();
    auto& gfxQueue = mGraphicsComponent.getTaskQueue();
    gfxQueue.executeQueue();
    collectGarbage();

    LOG_TRACE("\n\n=========================[ Update Complete ]=========================\n\n");
  }

  void
  ProjectRuntime::clearAllCaches
  ()
  {
    LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);

    mDestructionTaskQueue.executeQueue();
    auto& gfxDestQueue = mGraphicsComponent.getDestructionTaskQueue();
    gfxDestQueue.executeQueue();

    mAudioCache.clear();
    mModelCache.clear();
    mShaderCache.clear();
    mMaterialCache.clear();
    mTextureCache.clear();
    mScriptCache.clear();
    mFontCache.clear();
  }

  // Accessors  ==============================================================

  AudioCache&
  ProjectRuntime::getAudioCache
  ()
  {
    return mAudioCache;
  }

  ShaderCache&
  ProjectRuntime::getShaderCache
  ()
  {
    return mShaderCache;
  }

  MaterialCache&
  ProjectRuntime::getMaterialCache
  ()
  {
    return mMaterialCache;
  }

  ModelCache&
  ProjectRuntime::getModelCache
  ()
  {
    return mModelCache;
  }

  TextureCache&
  ProjectRuntime::getTextureCache
  ()
  {
    return mTextureCache;
  }

  ScriptCache&
  ProjectRuntime::getScriptCache
  ()
  {
    return mScriptCache;
  }

  FontCache&
  ProjectRuntime::getFontCache
  ()
  {
    return mFontCache;
  }

  bool
  ProjectRuntime::hasActiveSceneRuntime
  ()
  const
  {
    return mActiveSceneRuntime.has_value();
  }

  bool
  ProjectRuntime::hasLoadedScenes
  ()
  const
  {
    for (auto& srt : mSceneRuntimeVector)
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
    for (auto& srt : mSceneRuntimeVector)
    {
      if (srt->getUuid() == uuid) return true;
    }
    return false;
  }

  deque<float>
  ProjectRuntime::getFrameDurationHistory
  () const
  {
    return mFrameDurationHistory;
  }

  float
  ProjectRuntime::getAverageFramerate
  ()
  {
    float f = 0.0;
    for (const auto& dur : mFrameDurationHistory)
    {
      f += dur;
    }
    return f/MaxFrameCount;
  }

  TaskQueue<Task>&
  ProjectRuntime::getTaskQueue
  ()
  {
    return mTaskQueue;
  }

  TaskQueue<DestructionTask>&
  ProjectRuntime::getDestructionTaskQueue
  ()
  {
    return mDestructionTaskQueue;
  }

  WindowComponent&
  ProjectRuntime::getWindowComponent
  ()
  {
    return mWindowComponent;
  }

  void
  ProjectRuntime::setDone
  (bool done)
  {
    mDone = done;
  }

  Time&
  ProjectRuntime::getTime
  ()
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

  AudioComponent&
  ProjectRuntime::getAudioComponent
  ()
  {
    return mAudioComponent;
  }

  PhysicsComponent&
  ProjectRuntime::getPhysicsComponent
  ()
  {
    return mPhysicsComponent;
  }

  GraphicsComponent&
  ProjectRuntime::getGraphicsComponent
  ()
  {
    return mGraphicsComponent;
  }

  ScriptComponent&
  ProjectRuntime::getScriptComponent
  ()
  {
    return mScriptComponent;
  }

  StorageManager&
  ProjectRuntime::getStorageManager
  ()
  {
    return mStorageManager;
  }

  InputComponent&
  ProjectRuntime::getInputComponent
  ()
  {
    return mInputComponent;
  }

  // Scene =====================================================================

  SceneRuntime&
  ProjectRuntime::createSceneRuntime(SceneDefinition& sd)
  {
    return *mSceneRuntimeVector.emplace_back(make_unique<SceneRuntime>(*this, sd));
  }

  optional<reference_wrapper<SceneRuntime>>
  ProjectRuntime::getActiveSceneRuntime
  ()
  const
  {
    return mActiveSceneRuntime;
  }

  SceneRuntime&
  ProjectRuntime::getSceneRuntimeByUuid
  (UuidType uuid)
  {
    LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
    for (auto& sr : mSceneRuntimeVector)
    {
      if (sr->getUuid() == uuid)
      {
        return *sr;
      }
    }
    throw std::exception();
  }

  void
  ProjectRuntime::setActiveSceneRuntime
  (UuidType uuid)
  {
    LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
    mActiveSceneRuntime = getSceneRuntimeByUuid(uuid);
    if (mActiveSceneRuntime)
    {
      mActiveSceneRuntime.value().get().setState(SCENE_STATE_ACTIVE);
    }
  }

  vector<reference_wrapper<SceneRuntime>>
  ProjectRuntime::getSceneRuntimeVector
  ()
  {
    vector<reference_wrapper<SceneRuntime>> ret;
    for(auto& sr : mSceneRuntimeVector)
    {
      ret.push_back(*sr);
    }
    return ret;
  }

  void
  ProjectRuntime::removeSceneRuntime
  (const SceneRuntime& rt)
  {
    std::remove_if(mSceneRuntimeVector.begin(), mSceneRuntimeVector.end(),
		[&](unique_ptr<SceneRuntime>& next)
		{ return next->getUuid() == rt.getUuid();});
  }

  ProjectDirectory&
  ProjectRuntime::getProjectDirectory()
  {
    return mProjectDirectory;
  }

  unsigned int ProjectRuntime::MaxFrameCount = 100;
}
