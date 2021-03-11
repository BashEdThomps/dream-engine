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
    (ProjectRuntime* pr, EntityRuntime* rt)
        : Task(pr, "EntityScriptCreateStateTask"),
          mEntity(rt)
    {
    }

    void EntityScriptCreateStateTask::execute()
    {
        LOG_TRACE("EntityScriptCreateStateTask: Executing {}",getID());

        auto sr = mEntity->getScriptRuntime();
        if(sr->createEntityState(mEntity))
        {
            setState(TASK_STATE_COMPLETED);
        }
        else
        {
            setState(TASK_STATE_FAILED);
        }
    }

    // =========================================================================

    EntityScriptOnInitTask::EntityScriptOnInitTask
    (ProjectRuntime* pr, EntityRuntime* rt)
        : Task(pr, "EntityScriptExecuteOnInitTask"),
          mEntity(rt)
    {
    }

    void EntityScriptOnInitTask::execute()
    {
        LOG_TRACE("EntityScriptExecuteOnInitTask: Executing {}",getID());

        auto sr = mEntity->getScriptRuntime();
        if(sr->executeOnInit(mEntity))
        {
            setState(TASK_STATE_COMPLETED);
        }
        else
        {
            setState(TASK_STATE_FAILED);
        }
    }

    // =========================================================================

    EntityScriptOnUpdateTask::EntityScriptOnUpdateTask
    (ProjectRuntime* pr, EntityRuntime* rt)
        : Task(pr, "EntityScriptExecuteOnUpdateTask"),
          mEntity(rt)
    {
    }

    void EntityScriptOnUpdateTask::execute()
    {
        LOG_TRACE("EntityScriptOnUpdateTask: Executing {}",getID());

        auto sr = mEntity->getScriptRuntime();
        if(sr->executeOnUpdate(mEntity))
        {
            setState(TASK_STATE_COMPLETED);
        }
        else
        {
            setState(TASK_STATE_FAILED);
        }
    }

    // =========================================================================

    EntityScriptOnEventTask::EntityScriptOnEventTask
    (ProjectRuntime* pr, EntityRuntime* rt)
        : Task(pr, "EntityScriptOnEventTask"),
          mEntity(rt)
    {
        LOG_TRACE("EntityScriptOnEventTask: {}", __FUNCTION__);
    }

    void EntityScriptOnEventTask::execute()
    {
        LOG_TRACE("ScriptOnEventTask: Executing {}",getID());

        auto sr = mEntity->getScriptRuntime();
        if(sr->executeOnEvent(mEntity))
        {
            setState(TASK_STATE_COMPLETED);
        }
        else
        {
            setState(TASK_STATE_FAILED);
        }
    }

    // =========================================================================

    EntityScriptRemoveStateTask::EntityScriptRemoveStateTask
    (ProjectRuntime* pr,UuidType entityUuid,  ScriptRuntime* sr)
        : DestructionTask(pr, "EntityScriptRemoveStateTask"),
          mScript(sr),
          mEntityUuid(entityUuid)
    {
        LOG_DEBUG("EntityScriptRemoveStateTask: {}", __FUNCTION__);
    }

    void EntityScriptRemoveStateTask::execute()
    {
        LOG_TRACE("EntityScriptRemoveStateTask:Executing {}",getID());

        if(mScript->removeEntityState(mEntityUuid))
        {
            setState(TASK_STATE_COMPLETED);
        }
        else
        {
            setState(TASK_STATE_FAILED);
        }
    }
}

