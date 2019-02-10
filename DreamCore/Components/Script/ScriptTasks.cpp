#include "ScriptTasks.h"
#include "ScriptRuntime.h"
#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../Project/ProjectRuntime.h"

namespace Dream
{
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
            setState(TaskState::COMPLETED);
        }
        else
        {
            setState(TaskState::WAITING);
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
