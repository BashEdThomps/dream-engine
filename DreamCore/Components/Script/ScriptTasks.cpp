#include "ScriptTasks.h"

#include "ScriptRuntime.h"

#include "Common/Logger.h"
#include "Scene/SceneRuntime.h"
#include "Scene/Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"

namespace Dream
{
    ScriptConstructionTask::ScriptConstructionTask
    ()
        : Task()
     {
     }

    void
    ScriptConstructionTask::execute
    ()
    {
        LOG_CRITICAL("ScriptConstructionTask: Executing on thread {}",mThreadId);

        if(mScript->createScript())
        {
            setState(TaskState::COMPLETED);
            mScript->setLoaded(true);
        }
        else
        {
            setState(TaskState::WAITING);
            mDeferralCount++;
        }
    }

    void ScriptConstructionTask::setScript(ScriptRuntime *rt)
    {
        mScript = rt;
    }

     ScriptOnInitTask::ScriptOnInitTask
     (EntityRuntime* rt)
         : Task(),
           mEntity(rt)
     {
     }

    void
    ScriptOnInitTask::execute
    ()
    {
        LOG_CRITICAL("ScriptOnInitTask: Executing for SO {} script {} on thread {}",mEntity->getName(), mScript->getNameAndUuidString(), mThreadId);

        if(mScript->executeOnInit(mEntity))
        {
            setState(TaskState::COMPLETED);
        }
        else
        {
            setState(TaskState::WAITING);
            mDeferralCount++;
        }

    }

    void
    ScriptOnInitTask::setScript
    (ScriptRuntime *rt)
    {
        mScript = rt;
    }
//==================================================================================================

    ScriptOnUpdateTask::ScriptOnUpdateTask
    (EntityRuntime* rt)
        : Task(),
          mEntity(rt)
    {
    }

    void
    ScriptOnUpdateTask::execute
    ()
    {
        LOG_CRITICAL("ScriptOnUpdateTask: Executing on thread {}",mThreadId);

        if(mScript->executeOnUpdate(mEntity))
        {
            setState(TaskState::COMPLETED);
        }
        else
        {
            setState(TaskState::WAITING);
            mDeferralCount++;
        }
    }

    void
    ScriptOnUpdateTask::setScript
    (ScriptRuntime *rt)
    {
        mScript = rt;
    }

//==================================================================================================

    ScriptOnEventTask::ScriptOnEventTask
    (EntityRuntime* rt)
        : Task(),
          mEntity(rt)
    {
    }

    void
    ScriptOnEventTask::execute
    ()
    {
        LOG_CRITICAL("ScriptOnEventTask: Executing on thread {}",mThreadId);

        if(mScript->executeOnEvent(mEntity))
        {
            setState(TaskState::COMPLETED);
        }
        else
        {
            setState(TaskState::WAITING);
            mDeferralCount++;
        }
    }

    void
    ScriptOnEventTask::setScript
    (ScriptRuntime *rt)
    {
        mScript = rt;
    }

//==================================================================================================

    ScriptOnDestroyTask::ScriptOnDestroyTask
    (uint32_t destroyed, EntityRuntime* parent)
        : DestructionTask(),
          mDestroyedObject(destroyed),
          mParentEntity(parent)
    {
    }

    void
    ScriptOnDestroyTask::execute
    ()
    {
        LOG_CRITICAL(
            "ScriptOnDestroyTask: Executing onDestroy in script {} for SO {} (child of {}) on thread {}",
            mScript->getNameAndUuidString(),mDestroyedObject,
            mParentEntity->getNameAndUuidString(), mThreadId
        );

        if (mScript != nullptr)
        {
            if(mScript->executeOnDestroy(mDestroyedObject, mParentEntity))
            {
                setState(TaskState::COMPLETED);
            }
            else
            {
                setState(TaskState::WAITING);
                mDeferralCount++;
            }
        }
        else {
            setState(TaskState::COMPLETED);
        }
    }

    void
    ScriptOnDestroyTask::setScript
    (ScriptRuntime *rt)
    {
        mScript = rt;
    }
}
