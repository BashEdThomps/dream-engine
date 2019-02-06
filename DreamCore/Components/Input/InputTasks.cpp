#include "InputTasks.h"
#include "InputComponent.h"
#include "../../Scene/SceneRuntime.h"

namespace Dream
{
   InputPollDataTask::InputPollDataTask
   (InputComponent* cp)
       : Task(), mComponent(cp)
   {
    #ifdef DREAM_LOG
    setLogClassName("InputPollDataTask");
    #endif
    mComponent->setPollDataTask(this);
   }

   void
   InputPollDataTask::execute
   ()
   {
    if (mComponent->tryLock())
    {
        #ifdef DREAM_LOG
        getLog()->critical("Executing...");
        #endif
        mComponent->pollData();
        mComponent->setPollDataTask(nullptr);
        mComponent->unlock();
        mCompleted = true;
    }
    else
    {
        mDeferralCount++;
    }
   }

   InputExecuteScriptTask::InputExecuteScriptTask
   (InputComponent* cp, SceneRuntime* rt)
       : Task(), mComponent(cp), mSceneRuntime(rt)
   {
    #ifdef DREAM_LOG
    setLogClassName("InputExecuteScriptTask");
    #endif
    mComponent->setExecuteScriptTask(this);
   }

    void InputExecuteScriptTask::execute()
   {
       #ifdef DREAM_LOG
    getLog()->critical("Executing on thread {}",mThreadId);
    #endif

       if (mComponent->tryLock())
       {
        #ifdef DREAM_LOG
        getLog()->critical("Executing...");
        #endif

        if (mComponent->executeInputScript(mSceneRuntime))
        {
        mComponent->setExecuteScriptTask(nullptr);
        mCompleted = true;
        }
        else
        {
        mDeferralCount++;
        }
        mComponent->unlock();
       }
       else
       {
       mDeferralCount++;
       }
   }
}
