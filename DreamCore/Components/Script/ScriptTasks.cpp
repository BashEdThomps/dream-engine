#include "ScriptTasks.h"

#include "ScriptRuntime.h"
#include "Common/Logger.h"
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

        if(mScript->createEntityState(mEntity))
        {
            setState(TASK_STATE_COMPLETED);
        }
        else
        {
            setState(TASK_STATE_FAILED);
        }
    }

    void EntityScriptCreateStateTask::setScript(ScriptRuntime *rt)
    {
        mScript = rt;
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

        if(mScript->executeOnInit(mEntity))
        {
            setState(TASK_STATE_COMPLETED);
        }
        else
        {
            setState(TASK_STATE_FAILED);
        }
    }

    void EntityScriptOnInitTask::setScript(ScriptRuntime *rt)
    {
        mScript = rt;
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

        if(mScript->executeOnUpdate(mEntity))
        {
            setState(TASK_STATE_COMPLETED);
        }
        else
        {
            setState(TASK_STATE_FAILED);
        }
    }

    void EntityScriptOnUpdateTask::setScript(ScriptRuntime *rt)
    {
        mScript = rt;
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

        if(mScript->executeOnEvent(mEntity))
        {
            setState(TASK_STATE_COMPLETED);
        }
        else
        {
            setState(TASK_STATE_FAILED);
        }
    }

    void EntityScriptOnEventTask::setScript(ScriptRuntime *rt)
    {
        mScript = rt;
    }

    // =========================================================================

    EntityScriptRemoveStateTask::EntityScriptRemoveStateTask
    (ProjectRuntime* pr, UuidType uuid)
        : DestructionTask(pr, "EntityScriptRemoveStateTask"),
          mUuid(uuid)
    {
        LOG_DEBUG("EntityScriptRemoveStateTask: {}", __FUNCTION__);
    }

    void EntityScriptRemoveStateTask::execute()
    {
        LOG_TRACE("EntityScriptRemoveStateTask:Executing {}",getID());

        if(mScript->removeEntityState(mUuid))
        {
            setState(TASK_STATE_COMPLETED);
        }
        else
        {
            setState(TASK_STATE_FAILED);
        }
    }

    void EntityScriptRemoveStateTask::setScript(ScriptRuntime *rt)
    {
        mScript = rt;
    }

    // =========================================================================

    InputScriptCreateStateTask::InputScriptCreateStateTask
    (ProjectRuntime* pr)
        : Task(pr, "InputScriptCreateStateTask")
    {
    }

    void InputScriptCreateStateTask::execute()
    {
        LOG_TRACE("InputScriptCreateStateTask: Executing {}",getID());

        if(mScript->registerInputScript())
        {
            setState(TASK_STATE_COMPLETED);
        }
        else
        {
            setState(TASK_STATE_FAILED);
        }
    }

    void InputScriptCreateStateTask::setScript(ScriptRuntime *rt)
    {
        mScript = rt;
    }

    // =========================================================================

    InputScriptRemoveStateTask::InputScriptRemoveStateTask
    (ProjectRuntime* pr)
        : DestructionTask(pr, "InputScriptRemoveStateTask")
    {
        LOG_DEBUG("InputScriptRemoveStateTask: {}", __FUNCTION__);
    }

    void InputScriptRemoveStateTask::execute()
    {
        LOG_TRACE("InputScriptRemoveStateTask:Executing {}",getID());

        if(mScript->removeInputScript())
        {
            setState(TASK_STATE_COMPLETED);
        }
        else
        {
            setState(TASK_STATE_FAILED);
        }
    }

    void InputScriptRemoveStateTask::setScript(ScriptRuntime *rt)
    {
        mScript = rt;
    }
}

