#include "PhysicsTasks.h"

#include "Common/Logger.h"

#include "PhysicsObjectRuntime.h"
#include "PhysicsComponent.h"
#include "PhysicsDebugDrawer.h"

namespace octronic::dream
{
         PhysicsAddObjectTask::PhysicsAddObjectTask
         (PhysicsComponent* cp, PhysicsObjectRuntime* rt)
            : Task("PhysicsAddObjectTask"),
              mComponent(cp),
              mRuntime(rt)
        {
        }


        void
        PhysicsAddObjectTask::execute
        ()
        {
            LOG_TRACE("PhysicsAddObjectTask: Executing on thread {}",mThreadId);
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
            	LOG_TRACE("{}: Failed to lock target runtime",mTaskName);
                setState(TaskState::TASK_STATE_WAITING);
                mDeferralCount++;
            }
        }

        PhysicsUpdateWorldTask::PhysicsUpdateWorldTask
        (PhysicsComponent* cp)
            : Task("PhysicsUpdateWorldTask"), mComponent(cp)
        {
        }

        void
        PhysicsUpdateWorldTask::execute
        ()
        {
            LOG_TRACE("PhysicsUpdateWorldTask: Executing on thread {}",mThreadId);
            if(mComponent->tryLock())
            {
                mComponent->stepSimulation();
                mComponent->unlock();
                setState(TaskState::TASK_STATE_COMPLETED);
            }
            else
            {
            	LOG_TRACE("{}: Failed to lock target runtime",mTaskName);
                setState(TaskState::TASK_STATE_WAITING);
                mDeferralCount++;
            }
        }

        PhysicsDrawDebugTask::PhysicsDrawDebugTask
        (PhysicsComponent* cp)
            : GraphicsComponentTask("PhysicsDrawDebugTask"), mComponent(cp)
        {
        }

        void
        PhysicsDrawDebugTask::execute
        ()
        {
            LOG_TRACE("PhysicsDrawDebugTask: Executing on Graphics thread");
            if(mComponent->tryLock())
            {
                mComponent->getDebugDrawer()->drawAll();
                mComponent->unlock();
                setState(TaskState::TASK_STATE_COMPLETED);
            }
            else
            {
            	LOG_TRACE("{}: Failed to lock target runtime",mTaskName);
                setState(TaskState::TASK_STATE_WAITING);
            }
        }
}
