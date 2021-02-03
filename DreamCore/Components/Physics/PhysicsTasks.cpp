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
        LOG_TRACE("PhysicsAddObjectTask: Executing on thread {}",getThreadID());
        mComponent->addPhysicsObjectRuntime(mRuntime);
        mRuntime->setInPhysicsWorld(true);
        setState(TaskState::TASK_STATE_COMPLETED);
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
        LOG_TRACE("PhysicsUpdateWorldTask: {} Executing",getDebugString());
        mComponent->stepSimulation();
        setState(TaskState::TASK_STATE_COMPLETED);
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
        mComponent->getDebugDrawer()->drawAll();
        setState(TaskState::TASK_STATE_COMPLETED);
    }
}
