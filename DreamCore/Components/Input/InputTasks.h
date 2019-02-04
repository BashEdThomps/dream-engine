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
                mComponent->pollData();
                mComponent->setPollDataTask(nullptr);
                mComponent->unlock();
                clearDeferred();
            }
            else
            {
                setDeferred();
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
            setLogClassName("InputUpdateTask");
            #endif
            mComponent->setExecuteScriptTask(this);
       }

       inline void execute() override
       {
            mComponent->executeInputScript(mSceneRuntime);
            mComponent->setExecuteScriptTask(nullptr);
       }
    };

}
