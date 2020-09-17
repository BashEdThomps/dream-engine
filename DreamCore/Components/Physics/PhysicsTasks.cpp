#include "PhysicsTasks.h"
#include "PhysicsObjectRuntime.h"
#include "PhysicsComponent.h"
#include "PhysicsDebugDrawer.h"

namespace Dream
{
         PhysicsAddObjectTask::PhysicsAddObjectTask
         (PhysicsComponent* cp, PhysicsObjectRuntime* rt)
            : Task(),
              mComponent(cp),
              mRuntime(rt)
        {

            #ifdef DREAM_LOG
            setLogClassName("PhysicsAddObjectTask");
            #endif
        }


        void
        PhysicsAddObjectTask::execute
        ()
        {
            #ifdef DREAM_LOG
            getLog()->critical("Executing on thread {}",mThreadId);
            getLog()->trace("Adding Actor to physics world");
            #endif

            if(mComponent->tryLock())
            {
                    mComponent->addPhysicsObjectRuntime(mRuntime);
                    mComponent->unlock();
                    mRuntime->setInPhysicsWorld(true);
                    setState(TaskState::COMPLETED);
            }
            else
            {
                setState(TaskState::WAITING);
                mDeferralCount++;
            }
        }

        PhysicsUpdateWorldTask::PhysicsUpdateWorldTask
        (PhysicsComponent* cp)
            : Task(), mComponent(cp)
        {
            #ifdef DREAM_LOG
            setLogClassName("PhysicsComponentUpdateTask");
            #endif
        }

        void
        PhysicsUpdateWorldTask::execute
        ()
        {
            #ifdef DREAM_LOG
            getLog()->critical("Executing on thread {}",mThreadId);
            #endif
            if(mComponent->tryLock())
            {
                mComponent->stepSimulation();
                mComponent->unlock();
                setState(TaskState::COMPLETED);
            }
            else
            {
                setState(TaskState::WAITING);
                mDeferralCount++;
            }
        }

        PhysicsDrawDebugTask::PhysicsDrawDebugTask
        (PhysicsComponent* cp)
            : GraphicsComponentTask(), mComponent(cp)
        {
            #ifdef DREAM_LOG
            setLogClassName("PhysicsDrawDebugTask");
            #endif
        }

        void
        PhysicsDrawDebugTask::execute
        ()
        {
            #ifdef DREAM_LOG
            getLog()->critical("Executing on thread {}",mThreadId);
            #endif
            if(mComponent->tryLock())
            {
                mComponent->getDebugDrawer()->drawAll();
                mComponent->unlock();
                setState(TaskState::COMPLETED);
            }
            else
            {
                setState(TaskState::WAITING);
            }
        }
}
