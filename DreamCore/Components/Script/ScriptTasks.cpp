#pragma once

#include "ScriptTasks.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "ScriptRuntime.h"

namespace Dream
{
        ScriptCreateStateTask::ScriptCreateStateTask
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

         void ScriptCreateStateTask::execute()
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

         ScriptRemoveStateTask::ScriptRemoveStateTask
         (ScriptRuntime* script, uint32_t uuid)
            : Task(),
              mUuid(uuid),
              mScript(script)
        {
            #ifdef DREAM_LOG
            setLogClassName("ScriptRemoveStateTask");
            #endif
        }

         void ScriptRemoveStateTask::execute()
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

    void ScriptExecuteOnInitTask::execute() {}

    void ScriptExecuteOnUpdateTask::execute() {}

    void ScriptExecuteOnEventTask::execute() {}

    void ScriptExecuteOnNanoVGTask::execute() {}
}
