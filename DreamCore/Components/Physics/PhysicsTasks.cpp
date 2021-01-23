#include "PhysicsTasks.h"

#include "Common/Logger.h"

#include "PhysicsObjectRuntime.h"
#include "PhysicsComponent.h"
#include "PhysicsDebugDrawer.h"

namespace octronic::dream
{
         PhysicsAddObjectTask::PhysicsAddObjectTask
         (PhysicsComponent* cp, PhysicsObjectRuntime* rt)
            : Task(),
              mComponent(cp),
              mRuntime(rt)
        {
        }


        void
        PhysicsAddObjectTask::execute
        ()
        {
            LOG_CRITICAL("PhysicsAddObjectTask: Executing on thread {}",mThreadId);
            LOG_TRACE("PhysicsAddObjectTask: Adding Entity to physics world");

            if(mComponent->tryLock())
            {
                    mComponent->addPhysicsObjectRuntime(mRuntime);
                    mComponent->unlock();
                    mRuntime->setInPhysicsWorld(true);
                    setState(TaskState::TASK_STATE_COMPLETED);
            }
            else
            {
                setState(TaskState::TASK_STATE_WAITING);
                mDeferralCount++;
            }
        }

        PhysicsUpdateWorldTask::PhysicsUpdateWorldTask
        (PhysicsComponent* cp)
            : Task(), mComponent(cp)
        {
        }

        void
        PhysicsUpdateWorldTask::execute
        ()
        {
            LOG_CRITICAL("PhysicsUpdateWorldTask: Executing on thread {}",mThreadId);
            if(mComponent->tryLock())
            {
                mComponent->stepSimulation();
                mComponent->unlock();
                setState(TaskState::TASK_STATE_COMPLETED);
            }
            else
            {
                setState(TaskState::TASK_STATE_WAITING);
                mDeferralCount++;
            }
        }

        PhysicsDrawDebugTask::PhysicsDrawDebugTask
        (PhysicsComponent* cp)
            : GraphicsComponentTask(), mComponent(cp)
        {
        }

        void
        PhysicsDrawDebugTask::execute
        ()
        {
            LOG_CRITICAL("PhysicsDrawDebugTask: Executing on thread {}",mThreadId);
            if(mComponent->tryLock())
            {
                mComponent->getDebugDrawer()->drawAll();
                mComponent->unlock();
                setState(TaskState::TASK_STATE_COMPLETED);
            }
            else
            {
                setState(TaskState::TASK_STATE_WAITING);
            }
        }
}
