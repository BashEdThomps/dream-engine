#include "InputTasks.h"

#include "InputComponent.h"
#include "Common/Logger.h"
#include "Scene/SceneRuntime.h"
#include "Project/ProjectRuntime.h"

namespace octronic::dream
{

  // InputPollDataTask =======================================================

  InputPollDataTask::InputPollDataTask
  (ProjectRuntime& pr)
    : Task(pr, "InputPollDataTask")
  {
  }

  void
  InputPollDataTask::execute
  ()
  {
    LOG_TRACE("InputPollDataTask: Executing {}",getID());
    auto& inputComp = getProjectRuntime().getInputComponent();
    inputComp.pollData();
    setState(TaskState::TASK_STATE_COMPLETED);
  }

  // InputRegisterScriptTask =================================================

  InputRegisterScriptTask::InputRegisterScriptTask
  (ProjectRuntime& pr)
    : Task(pr, "InputRegisterScriptTask")
  {

  }

  void
  InputRegisterScriptTask::execute
  ()
  {
    LOG_TRACE("InputRegisterScriptTask: Executing {}",getID());
    auto& inputComp = getProjectRuntime().getInputComponent();
    if (inputComp.registerInputScript())
    {
      setState(TaskState::TASK_STATE_COMPLETED);
      return;
    }
  }

  // InputExecuteScriptTask ==================================================

  InputExecuteScriptTask::InputExecuteScriptTask
  (ProjectRuntime& pr)
    : Task(pr, "InputExecuteScriptTask")
  {
  }

  void
  InputExecuteScriptTask::execute
  ()
  {
    auto& inputComp = getProjectRuntime().getInputComponent();
    LOG_TRACE("InputExecuteScriptTask: Executing {}",getID());

    if (inputComp.executeInputScript())
    {
      setState(TaskState::TASK_STATE_COMPLETED);
      return;
    }
  }

  // InputRemoveScriptTask ===================================================

  InputRemoveScriptTask::InputRemoveScriptTask
  (ProjectRuntime& pr)
    : DestructionTask(pr, "InputRemoveScriptTask")
  {

  }

  void
  InputRemoveScriptTask::execute
  ()
  {
    LOG_TRACE("InputRemoveScriptTask: Executing {}",getID());
    auto& inputComp = getProjectRuntime().getInputComponent();
    if (inputComp.removeInputScript(mInputScriptUuid))
    {
      setState(TaskState::TASK_STATE_COMPLETED);
      return;
    }
  }

  void InputRemoveScriptTask::setInputScriptUuid(UuidType u)
  {
    mInputScriptUuid = u;
  }
}
