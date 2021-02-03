#include "ScriptTasks.h"

#include "ScriptRuntime.h"
#include "Common/Logger.h"
#include "Scene/SceneRuntime.h"
#include "Scene/Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"

namespace octronic::dream
{
    // =========================================================================

    EntityScriptConstructionTask::EntityScriptConstructionTask
    (EntityRuntime* rt)
        : Task("EntityScriptConstructionTask"),
          mEntity(rt)
    {
    }

    void EntityScriptConstructionTask::execute()
    {
        LOG_TRACE("EntityScriptConstructionTask: Executing on thread {}",getThreadID());

        if(mScript->createEntityState(mEntity))
        {
            setState(TaskState::TASK_STATE_COMPLETED);
        }
        else
        {
            incrementDeferralCount();
        }
    }

    void EntityScriptConstructionTask::setScript(ScriptRuntime *rt)
    {
        mScript = rt;
    }


    // =========================================================================

    EntityScriptOnInitTask::EntityScriptOnInitTask (EntityRuntime* rt)
        : Task("EntityScriptExecuteOnInitTask"),
          mEntity(rt)
    {
    }

    void EntityScriptOnInitTask::execute()
    {
        LOG_TRACE("EntityScriptExecuteOnInitTask: Executing on thread {}",getThreadID());

        if(mScript->executeOnInit(mEntity))
        {
            setState(TaskState::TASK_STATE_COMPLETED);
        }
        else
        {
            incrementDeferralCount();
        }
    }

    void EntityScriptOnInitTask::setScript(ScriptRuntime *rt)
    {
        mScript = rt;
    }

    // =========================================================================

    EntityScriptOnUpdateTask::EntityScriptOnUpdateTask(EntityRuntime* rt)
        : Task("EntityScriptExecuteOnUpdateTask"),
          mEntity(rt)
    {
    }

    void EntityScriptOnUpdateTask::execute()
    {
        LOG_TRACE("EntityScriptOnUpdateTask: Executing on thread {}",getThreadID());

        if(mScript->executeOnUpdate(mEntity))
        {
            setState(TaskState::TASK_STATE_COMPLETED);
        }
        else
        {
            incrementDeferralCount();
        }
    }

    void EntityScriptOnUpdateTask::setScript(ScriptRuntime *rt)
    {
        mScript = rt;
    }

    // =========================================================================

    EntityScriptOnEventTask::EntityScriptOnEventTask(EntityRuntime* rt)
        : Task("EntityScriptOnEventTask"),
          mEntity(rt)
    {
        LOG_TRACE("EntityScriptOnEventTask: {}", __FUNCTION__);
    }

    void EntityScriptOnEventTask::execute()
    {
        LOG_TRACE("ScriptOnEventTask: Executing on thread {}",getThreadID());

        if(mScript->executeOnEvent(mEntity))
        {
            setState(TaskState::TASK_STATE_COMPLETED);
        }
        else
        {
            incrementDeferralCount();
        }
    }

    void EntityScriptOnEventTask::setScript(ScriptRuntime *rt)
    {
        mScript = rt;
    }

    // =========================================================================

    EntityScriptDestructionTask::EntityScriptDestructionTask
    (UuidType uuid)
        : DestructionTask("EntityScriptDestructionTask"),
          mUuid(uuid)
    {
        LOG_DEBUG("EntityScriptDestructionTask: {}", __FUNCTION__);
    }

    void EntityScriptDestructionTask::execute()
    {
        LOG_TRACE("EntityScriptDestructionTask:Executing on thread {}",getThreadID());

        if(mScript->removeEntityState(mUuid))
        {
            setState(TaskState::TASK_STATE_COMPLETED);
        }
        else
        {
            incrementDeferralCount();
        }
    }

    void EntityScriptDestructionTask::setScript(ScriptRuntime *rt)
    {
        mScript = rt;
    }

    // =========================================================================

    InputScriptConstructionTask::InputScriptConstructionTask
    ()
        : Task("InputScriptConstructionTask")
    {
    }

    void InputScriptConstructionTask::execute()
    {
        LOG_TRACE("InputScriptConstructionTask: Executing on thread {}",getThreadID());

        if(mScript->registerInputScript())
        {
            setState(TaskState::TASK_STATE_COMPLETED);
        }
        else
        {
            incrementDeferralCount();
        }
    }

    void InputScriptConstructionTask::setScript(ScriptRuntime *rt)
    {
        mScript = rt;
    }

    // =========================================================================

    InputScriptDestructionTask::InputScriptDestructionTask()
        : DestructionTask("InputScriptDestructionTask")
    {
        LOG_DEBUG("InputScriptDestructionTask: {}", __FUNCTION__);
    }

    void InputScriptDestructionTask::execute()
    {
        LOG_TRACE("InputScriptDestructionTask:Executing on thread {}",getThreadID());

        if(mScript->removeInputScript())
        {
            setState(TaskState::TASK_STATE_COMPLETED);
        }
        else
        {
            incrementDeferralCount();
        }
    }

    void InputScriptDestructionTask::setScript(ScriptRuntime *rt)
    {
        mScript = rt;
    }
}

