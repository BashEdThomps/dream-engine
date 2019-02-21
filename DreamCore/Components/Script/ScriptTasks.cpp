﻿#include "ScriptTasks.h"
#include "ScriptRuntime.h"
#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
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
     (SceneObjectRuntime* rt)
         : Task(),
           mSceneObject(rt)
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
        getLog()->critical("Executing for SO {} script {} on thread {}",mSceneObject->getName(), mScript->getNameAndUuidString(), mThreadId);
        #endif

        if(mScript->executeOnInit(mSceneObject))
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
    (SceneObjectRuntime* rt)
        : Task(),
          mSceneObject(rt)
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

        if(mScript->executeOnUpdate(mSceneObject))
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
    (SceneObjectRuntime* rt)
        : Task(),
          mSceneObject(rt)
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

        if(mScript->executeOnEvent(mSceneObject))
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
    (uint32_t destroyed, SceneObjectRuntime* parent)
        : DestructionTask(),
          mDestroyedObject(destroyed),
          mParentSceneObject(parent)
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
            mParentSceneObject->getNameAndUuidString(), mThreadId
        );
        #endif

        if (mScript != nullptr)
        {
            if(mScript->executeOnDestroy(mDestroyedObject, mParentSceneObject))
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
