#include "PhysicsTasks.h"

#include "Common/Logger.h"

#include "PhysicsObjectRuntime.h"
#include "PhysicsComponent.h"

namespace octronic::dream
{
    PhysicsAddObjectTask::PhysicsAddObjectTask
    (const weak_ptr<ProjectRuntime>& pr,
     const weak_ptr<PhysicsComponent>& cp,
     const weak_ptr<PhysicsObjectRuntime>& rt)
        : Task(pr, "PhysicsAddObjectTask"),
          mComponent(cp),
          mRuntime(rt)
    {
    }


    void
    PhysicsAddObjectTask::execute
    ()
    {
      if (auto pcLock = mComponent.lock())
      {
        if (auto rtLock = mRuntime.lock())
        {
        LOG_TRACE("PhysicsAddObjectTask: Executing {}",getID());
        pcLock->addPhysicsObjectRuntime(mRuntime);
        rtLock->setInPhysicsWorld(true);
        setState(TaskState::TASK_STATE_COMPLETED);
        return;
        }
      }
      setState(TaskState::TASK_STATE_FAILED);
    }

    PhysicsUpdateWorldTask::PhysicsUpdateWorldTask
    (const weak_ptr<ProjectRuntime>& pr,
     const weak_ptr<PhysicsComponent>& cp)
        : Task(pr, "PhysicsUpdateWorldTask"), mComponent(cp)
    {
    }

    void
    PhysicsUpdateWorldTask::execute
    ()
    {
      if (auto pcLock = mComponent.lock())
      {
        LOG_TRACE("PhysicsUpdateWorldTask: {} Executing",getName());
        pcLock->stepSimulation();
        setState(TASK_STATE_COMPLETED);
        return;
      }
      setState(TASK_STATE_FAILED);
    }
}
