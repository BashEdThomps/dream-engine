#include "PhysicsTasks.h"

#include "Common/Logger.h"

#include "PhysicsObjectRuntime.h"
#include "PhysicsComponent.h"
#include "PhysicsDebugDrawer.h"

namespace octronic::dream
{
    PhysicsAddObjectTask::PhysicsAddObjectTask
    (ProjectRuntime* pr, PhysicsComponent* cp, PhysicsObjectRuntime* rt)
        : Task(pr, "PhysicsAddObjectTask"),
          mComponent(cp),
          mRuntime(rt)
    {
    }


    void
    PhysicsAddObjectTask::execute
    ()
    {
        LOG_TRACE("PhysicsAddObjectTask: Executing {}",getID());
        mComponent->addPhysicsObjectRuntime(mRuntime);
        mRuntime->setInPhysicsWorld(true);
        setState(TaskState::TASK_STATE_COMPLETED);
    }

    PhysicsUpdateWorldTask::PhysicsUpdateWorldTask
    (ProjectRuntime* pr, PhysicsComponent* cp)
        : Task(pr, "PhysicsUpdateWorldTask"), mComponent(cp)
    {
    }

    void
    PhysicsUpdateWorldTask::execute
    ()
    {
        LOG_TRACE("PhysicsUpdateWorldTask: {} Executing",getName());
        mComponent->stepSimulation();
        setState(TASK_STATE_COMPLETED);
    }

    PhysicsDrawDebugTask::PhysicsDrawDebugTask
    (ProjectRuntime* pr, PhysicsComponent* cp)
        : GraphicsTask(pr,"PhysicsDrawDebugTask"), mComponent(cp)
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
