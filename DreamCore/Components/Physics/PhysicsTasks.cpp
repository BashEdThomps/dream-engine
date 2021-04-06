#include "PhysicsTasks.h"

#include "Common/Logger.h"
#include "Project/ProjectRuntime.h"

#include "PhysicsObjectRuntime.h"
#include "PhysicsComponent.h"

namespace octronic::dream
{
  // PhysicsAddObjectTask ======================================================

  PhysicsAddObjectTask::PhysicsAddObjectTask
  (ProjectRuntime& pr, PhysicsObjectRuntime& rt)
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
    pComp.addPhysicsObjectRuntime(mRuntime);
    getPoRuntime().setInPhysicsWorld(true);
    setState(TaskState::TASK_STATE_COMPLETED);
  }

  PhysicsObjectRuntime& PhysicsAddObjectTask::getPoRuntime() const
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
