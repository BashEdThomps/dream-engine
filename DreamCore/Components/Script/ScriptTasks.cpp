#include "ScriptTasks.h"
#include "ScriptRuntime.h"
#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../Project/ProjectRuntime.h"

namespace Dream
{
//==================================================================================================
    ScriptCreateStateTask::ScriptCreateStateTask
    (SceneObjectRuntime* rt)
        : Task(),
          mSceneObject(rt)
    {
        #ifdef DREAM_LOG
        setLogClassName("ScriptCreateStateTask");
        #endif
    }

     void ScriptCreateStateTask::execute()
    {
        #ifdef DREAM_LOG
        getLog()->critical("Executing on thread {}",mThreadId);
        #endif

        if(mScript->createState(mSceneObject))
        {
            mState = TaskState::COMPLETED;
        }
        else
        {
            mState = TaskState::WAITING;
            mDeferralCount++;
        }
     }

     void ScriptCreateStateTask::setScript(ScriptRuntime *rt)
     {
        mScript = rt;
     }

//==================================================================================================
     ScriptRemoveStateTask::ScriptRemoveStateTask
     (uint32_t uuid)
        : Task(),
          mUuid(uuid)
    {
        #ifdef DREAM_LOG
        setLogClassName("ScriptRemoveStateTask");
        #endif
    }

    void
    ScriptRemoveStateTask::execute
    ()
    {
        #ifdef DREAM_LOG
        getLog()->critical("Executing on thread {}",mThreadId);
        #endif

        if(mScript->removeState(mUuid))
        {
            mState = TaskState::COMPLETED;
        }
        else
        {
            mState = TaskState::WAITING;
            mDeferralCount++;
        }
     }

     void
     ScriptRemoveStateTask::setScript
     (ScriptRuntime *rt)
     {
        mScript = rt;
     }

//==================================================================================================

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
        getLog()->critical("Executing on thread {}",mThreadId);
        #endif

        if(mScript->executeOnInit(mSceneObject->getScriptRuntimeState()))
        {
            mState = TaskState::COMPLETED;
        }
        else
        {
            mState = TaskState::WAITING;
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

        if(mScript->executeOnUpdate(mSceneObject->getScriptRuntimeState()))
        {
            mState = TaskState::COMPLETED;
        }
        else
        {
            mState = TaskState::WAITING;
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

        if(mScript->executeOnEvent(mSceneObject->getScriptRuntimeState()))
        {
            mState = TaskState::COMPLETED;
        }
        else
        {
            mState = TaskState::WAITING;
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

    ScriptOnNanoVGTask::ScriptOnNanoVGTask
    (SceneObjectRuntime* rt)
        : GraphicsComponentTask(),
          mSceneObject(rt)

    {
        #ifdef DREAM_LOG
        setLogClassName("ScriptExecuteOnNanoVGTask");
        #endif
    }

    void
    ScriptOnNanoVGTask::execute
    ()
    {
        #ifdef DREAM_LOG
        getLog()->critical("Executing on thread {}",mThreadId);
        #endif

        auto scene = mSceneObject->getSceneRuntime();
        auto project = scene->getProjectRuntime();
        auto nvg = project->getNanoVGComponent();

        if(mScript->executeOnNanoVG(nvg,scene))
        {
            mState = TaskState::COMPLETED;
        }
        else
        {
            mState = TaskState::WAITING;
            mDeferralCount++;
        }
    }

    void
    ScriptOnNanoVGTask::setScript
    (ScriptRuntime *rt)
    {
        mScript = rt;
    }
}
