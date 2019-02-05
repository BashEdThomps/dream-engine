#pragma once

#include "../../TaskManager/Task.h"
#include "../Graphics/GraphicsComponentTask.h"
#include "PhysicsObjectRuntime.h"
#include "PhysicsComponent.h"
#include "PhysicsDebugDrawer.h"

namespace Dream
{
    class PhysicsAddObjectTask : public Task
    {
        PhysicsObjectRuntime* mRuntime;
        PhysicsComponent* mComponent;
    public:
        inline PhysicsAddObjectTask(PhysicsComponent* cp, PhysicsObjectRuntime* rt)
            : Task(),
              mComponent(cp),
              mRuntime(rt)
        {

            #ifdef DREAM_LOG
            setLogClassName("PhysicsAddObjectTask");
            #endif
            mRuntime->setAddObjectTask(this);
        }

        inline void execute() override
        {
            #ifdef DREAM_LOG
            getLog()->critical("Executing on thread {}",mThreadId);
            getLog()->trace("Adding SceneObject to physics world");
            #endif
            if(mRuntime->tryLock())
            {
                if(mComponent->tryLock())
                {
                    mComponent->addPhysicsObjectRuntime(mRuntime);
                    mComponent->unlock();
                    mRuntime->setInPhysicsWorld(true);
                    mRuntime->setAddObjectTask(nullptr);
                    mCompleted=true;
                }
                else
                {
                    mDeferralCount++;
                }
                mRuntime->unlock();
            }
            else
            {
                mDeferralCount++;
            }
        }
    };

    class PhysicsUpdateWorldTask : public Task
    {
        PhysicsComponent* mComponent;
    public:
        PhysicsUpdateWorldTask(PhysicsComponent* cp)
            : Task(), mComponent(cp)
        {
            #ifdef DREAM_LOG
            setLogClassName("PhysicsComponentUpdateTask");
            #endif
            mComponent->setUpdateWorldTask(this);
        }

        inline void execute() override
        {
            #ifdef DREAM_LOG
            getLog()->critical("Executing on thread {}",mThreadId);
            #endif
            if(mComponent->tryLock())
            {
                mComponent->stepSimulation();
                mComponent->setUpdateWorldTask(nullptr);
                mComponent->unlock();
                mCompleted = true;
            }
            else
            {
                mDeferralCount++;
            }
        }
    };

    class PhysicsDrawDebugTask : public GraphicsComponentTask
    {
        PhysicsComponent* mComponent;
    public:
        inline PhysicsDrawDebugTask
        (PhysicsComponent* cp)
            : GraphicsComponentTask(), mComponent(cp)
        {
            #ifdef DREAM_LOG
            setLogClassName("PhysicsDrawDebugTask");
            #endif
            mComponent->setDrawDebugTask(this);
        }

        inline void execute() override
        {
            #ifdef DREAM_LOG
            getLog()->critical("Executing on thread {}",mThreadId);
            #endif
            if(mComponent->tryLock())
            {
                mComponent->getDebugDrawer()->drawAll();
                mComponent->setDrawDebugTask(nullptr);
                mComponent->unlock();
                mCompleted = true;
            }
            else
            {
                mDeferralCount++;
            }
        }
    };
}
