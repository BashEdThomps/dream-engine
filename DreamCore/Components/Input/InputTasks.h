#pragma once

#include "InputComponent.h"
#include "../../TaskManager/Task.h"

namespace Dream
{
    class InputPollDataTask : public Task
    {
        InputComponent* mComponent;
    public:
       inline InputPollDataTask(InputComponent* cp)
           : Task(), mComponent(cp)
       {
            #ifdef DREAM_LOG
            setLogClassName("InputPollDataTask");
            #endif
            mComponent->setPollDataTask(this);
       }

       inline void execute() override
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
    };

    class InputExecuteScriptTask : public Task
    {
        InputComponent* mComponent;
        SceneRuntime* mSceneRuntime;
    public:
       inline InputExecuteScriptTask(InputComponent* cp, SceneRuntime* rt)
           : Task(), mComponent(cp), mSceneRuntime(rt)
       {
            #ifdef DREAM_LOG
            setLogClassName("InputExecuteScriptTask");
            #endif
            mComponent->setExecuteScriptTask(this);
       }

       inline void execute() override
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
    };

}
