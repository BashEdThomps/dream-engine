#include "PhysicsTasks.h"

#include "Common/Logger.h"
#include "Project/ProjectRuntime.h"

#include "PhysicsRuntime.h"
#include "PhysicsComponent.h"

namespace octronic::dream
{
  // PhysicsAddObjectTask ======================================================

  PhysicsAddObjectTask::PhysicsAddObjectTask
  (ProjectRuntime& pr, PhysicsRuntime& rt)
    : Task(pr, "PhysicsAddObjectTask"),
      mRuntime(rt)
  {
  }


  void
  PhysicsAddObjectTask::execute
  ()
  {
    auto& pComp = getProjectRuntime().getPhysicsComponent();
    LOG_TRACE("PhysicsAddObjectTask: Executing {}",getID());
    pComp.addPhysicsRuntime(mRuntime);
    getPhysicsRuntime().setInPhysicsWorld(true);
    setState(TaskState::TASK_STATE_COMPLETED);
  }

  PhysicsRuntime&
  PhysicsAddObjectTask::getPhysicsRuntime
  ()
  const
  {
    return mRuntime.get();
  }

  // PhysicsUpdateWorldTask ====================================================

  PhysicsUpdateWorldTask::PhysicsUpdateWorldTask
  (ProjectRuntime& pr)
    : Task(pr, "PhysicsUpdateWorldTask")
  {
  }

  void
  PhysicsUpdateWorldTask::execute
  ()
  {
    auto& pComp = getProjectRuntime().getPhysicsComponent();
    LOG_TRACE("PhysicsUpdateWorldTask: {} Executing",getName());
    pComp.stepSimulation();
    setState(TASK_STATE_COMPLETED);
    return;
  }
}
