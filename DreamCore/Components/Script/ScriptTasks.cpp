#include "ScriptTasks.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "ScriptRuntime.h"

namespace Dream
{
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
                mCompleted = true;
            }
            else
            {
                mDeferralCount++;
            }
         }

         void ScriptCreateStateTask::setScript(ScriptRuntime *rt)
         {
            mScript = rt;
         }

         ScriptRemoveStateTask::ScriptRemoveStateTask
         (uint32_t uuid)
            : Task(),
              mUuid(uuid)
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

         void ScriptRemoveStateTask::setScript(ScriptRuntime *rt)
         {
            mScript = rt;
         }

    void ScriptExecuteOnInitTask::execute() {}

    void ScriptExecuteOnUpdateTask::execute() {}

    void ScriptExecuteOnEventTask::execute() {}

    void ScriptExecuteOnNanoVGTask::execute() {}
}
