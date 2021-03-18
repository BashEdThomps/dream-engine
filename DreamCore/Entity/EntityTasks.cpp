#include "EntityTasks.h"

#include "Common/Logger.h"
#include "Components/Script/ScriptRuntime.h"
#include "Scene/SceneRuntime.h"
#include "Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"

namespace octronic::dream
{
  // =========================================================================

  EntityScriptCreateStateTask::EntityScriptCreateStateTask
  (const weak_ptr<ProjectRuntime>& pr,
   const weak_ptr<EntityRuntime>& rt)
    : Task(pr, "EntityScriptCreateStateTask"),
      mEntity(rt)
  {
  }

  void EntityScriptCreateStateTask::execute()
  {
    LOG_TRACE("EntityScriptCreateStateTask: Executing {}",getID());

    if (auto entityLock = mEntity.lock())
    {
      if (auto sr = entityLock->getScriptRuntime().lock())
      {
        if(sr->createEntityState(mEntity))
        {
          setState(TASK_STATE_COMPLETED);
        }
        else
        {
          setState(TASK_STATE_FAILED);
        }
      }
    }
  }

  // =========================================================================

  EntityScriptOnInitTask::EntityScriptOnInitTask
  (const weak_ptr<ProjectRuntime>& pr,
   const weak_ptr<EntityRuntime>& rt)
    : Task(pr, "EntityScriptExecuteOnInitTask"),
      mEntity(rt)
  {
  }

  void EntityScriptOnInitTask::execute()
  {
    LOG_TRACE("EntityScriptExecuteOnInitTask: Executing {}",getID());

    if (auto entity = mEntity.lock())
    {
      if (auto sr = entity->getScriptRuntime().lock())
      {
        if(sr->executeOnInit(mEntity))
        {
          setState(TASK_STATE_COMPLETED);
        }
        else
        {
          setState(TASK_STATE_FAILED);
        }
      }
    }
  }

  // =========================================================================

  EntityScriptOnUpdateTask::EntityScriptOnUpdateTask
  (const weak_ptr<ProjectRuntime>& pr,
   const weak_ptr<EntityRuntime>& rt)
    : Task(pr, "EntityScriptExecuteOnUpdateTask"),
      mEntity(rt)
  {
  }

  void EntityScriptOnUpdateTask::execute()
  {
    LOG_TRACE("EntityScriptOnUpdateTask: Executing {}",getID());

    if (auto entityLock = mEntity.lock())
    {
      if (auto sr = entityLock->getScriptRuntime().lock())
      {
        if(sr->executeOnUpdate(mEntity))
        {
          setState(TASK_STATE_COMPLETED);
        }
        else
        {
          setState(TASK_STATE_FAILED);
        }
      }
    }
  }

  // =========================================================================

  EntityScriptOnEventTask::EntityScriptOnEventTask
  (const weak_ptr<ProjectRuntime>& pr,
   const weak_ptr<EntityRuntime>& rt)
    : Task(pr, "EntityScriptOnEventTask"),
      mEntity(rt)
  {
    LOG_TRACE("EntityScriptOnEventTask: {}", __FUNCTION__);
  }

  void EntityScriptOnEventTask::execute()
  {
    LOG_TRACE("ScriptOnEventTask: Executing {}",getID());

    if (auto entityLock = mEntity.lock())
    {
      if (auto sr = entityLock->getScriptRuntime().lock())
      {
        if(sr->executeOnEvent(mEntity))
        {
          setState(TASK_STATE_COMPLETED);
        }
        else
        {
          setState(TASK_STATE_FAILED);
        }
      }
    }
  }

  // =========================================================================

  EntityScriptRemoveStateTask::EntityScriptRemoveStateTask
  (const weak_ptr<ProjectRuntime>& pr,
   UuidType entityUuid,
   const weak_ptr<ScriptRuntime>& sr)
    : DestructionTask(pr, "EntityScriptRemoveStateTask"),
      mScript(sr),
      mEntityUuid(entityUuid)
  {
    LOG_DEBUG("EntityScriptRemoveStateTask: {}", __FUNCTION__);
  }

  void EntityScriptRemoveStateTask::execute()
  {
    LOG_TRACE("EntityScriptRemoveStateTask:Executing {}",getID());

    if (auto scriptLock = mScript.lock())
    {
      if(scriptLock->removeEntityState(mEntityUuid))
      {
        setState(TASK_STATE_COMPLETED);
      }
      else
      {
        setState(TASK_STATE_FAILED);
      }
    }
  }
}

