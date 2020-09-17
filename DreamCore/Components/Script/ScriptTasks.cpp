#include "ScriptTasks.h"
#include "ScriptRuntime.h"
#include "../../Scene/SceneRuntime.h"
#include "../../Scene/Actor/ActorRuntime.h"
#include "../../Project/ProjectRuntime.h"

namespace Dream
{
    ScriptConstructionTask::ScriptConstructionTask
    ()
        : Task()
     {
        #ifdef DREAM_LOG
        setLogClassName("ScriptConstructionTask");
        #endif
     }

    void
    ScriptConstructionTask::execute
    ()
    {
        #ifdef DREAM_LOG
        getLog()->critical("Executing on thread {}",mThreadId);
        #endif

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
     (ActorRuntime* rt)
         : Task(),
           mActor(rt)
     {
        #ifdef DREAM_LOG
        setLogClassName("ScriptExecuteOnInitTask");
        #endif
     }

    void
    ScriptOnInitTask::execute
    ()
    {
        #ifdef DREAM_LOG
        getLog()->critical("Executing for SO {} script {} on thread {}",mActor->getName(), mScript->getNameAndUuidString(), mThreadId);
        #endif

        if(mScript->executeOnInit(mActor))
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
    (ActorRuntime* rt)
        : Task(),
          mActor(rt)
    {
        #ifdef DREAM_LOG
        setLogClassName("ScriptExecuteOnUpdateTask");
        #endif
    }

    void
    ScriptOnUpdateTask::execute
    ()
    {
        #ifdef DREAM_LOG
        getLog()->critical("Executing on thread {}",mThreadId);
        #endif

        if(mScript->executeOnUpdate(mActor))
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
    (ActorRuntime* rt)
        : Task(),
          mActor(rt)
    {
        #ifdef DREAM_LOG
        setLogClassName("ScriptOnEventTask");
        #endif
    }

    void
    ScriptOnEventTask::execute
    ()
    {
        #ifdef DREAM_LOG
        getLog()->critical("Executing on thread {}",mThreadId);
        #endif

        if(mScript->executeOnEvent(mActor))
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
    (uint32_t destroyed, ActorRuntime* parent)
        : DestructionTask(),
          mDestroyedObject(destroyed),
          mParentActor(parent)
    {
        #ifdef DREAM_LOG
        setLogClassName("ScriptOnDestroyTask");
        #endif
    }

    void
    ScriptOnDestroyTask::execute
    ()
    {
        #ifdef DREAM_LOG
        getLog()->critical(
            "Executing onDestroy in script {} for SO {} (child of {}) on thread {}",
            mScript->getNameAndUuidString(),mDestroyedObject,
            mParentActor->getNameAndUuidString(), mThreadId
        );
        #endif

        if (mScript != nullptr)
        {
            if(mScript->executeOnDestroy(mDestroyedObject, mParentActor))
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
