#pragma once

#include "../../TaskManager/Task.h"
#include "../Graphics/GraphicsComponentTask.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "ScriptRuntime.h"

namespace Dream
{
    class ScriptCreateStateTask : public Task
    {
        SceneObjectRuntime* mSceneObject;
        ScriptRuntime* mScript;
    public:
        inline ScriptCreateStateTask
        (SceneObjectRuntime* rt, ScriptRuntime* script)
            : Task(),
              mSceneObject(rt),
              mScript(script)
        {
            #ifdef DREAM_LOG
            setLogClassName("ScriptCreateStateTask");
            #endif
            mSceneObject->setScriptCreateStateTask(this);
        }

        inline void execute() override
        {
            #ifdef DREAM_LOG
            getLog()->critical("Executing on thread {}",mThreadId);
            #endif

            if(mScript->createState(mSceneObject))
            {
                mSceneObject->setScriptCreateStateTask(nullptr);
                mCompleted = true;
            }
            else
            {
                mDeferralCount++;
            }
        }
    };

    class ScriptRemoveStateTask : public Task
    {
        uint32 mUuid;
        ScriptRuntime* mScript;
    public:
        inline ScriptRemoveStateTask
        (ScriptRuntime* script, uint32 uuid)
            : Task(),
              mScript(script),
              mUuid(uuid)
        {
            #ifdef DREAM_LOG
            setLogClassName("ScriptRemoveStateTask");
            #endif
        }

        inline void execute() override
        {
            #ifdef DREAM_LOG
            getLog()->critical("Executing on thread {}",mThreadId);
            #endif

            if(mScript->removeState(mUuid))
            {
                mCompleted = true;
            }
            else
            {
                mDeferralCount++;
            }
        }
    };

    class ScriptExecuteOnInitTask : public Task
    {
        inline void execute() override {}
    };

    class ScriptExecuteOnUpdateTask : public Task
    {
        inline void execute() override {}
    };

    class ScriptExecuteOnEventTask : public Task
    {
        inline void execute() override {}
    };

    class ScriptExecuteOnNanoVGTask : public GraphicsComponentTask
    {
        inline void execute() override {}
    };

}
