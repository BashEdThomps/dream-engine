#include "InputTasks.h"

#include "InputComponent.h"
#include "Common/Logger.h"
#include "Scene/SceneRuntime.h"
#include "Project/ProjectRuntime.h"

namespace octronic::dream
{

  // InputPollDataTask =======================================================

  InputPollDataTask::InputPollDataTask
  (const shared_ptr<ProjectRuntime>& pr)
    : Task(pr, "InputPollDataTask")
  {
  }

  void
  InputPollDataTask::execute
  ()
  {
    LOG_TRACE("InputPollDataTask: Executing {}",getID());
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto inputComp = prLock->getInputComponent().lock())
      {
        inputComp->pollData();
        setState(TaskState::TASK_STATE_COMPLETED);
      }
    }
    setState(TASK_STATE_FAILED);
  }

  // InputRegisterScriptTask =================================================

  InputRegisterScriptTask::InputRegisterScriptTask
  (const shared_ptr<ProjectRuntime>& pr)
    : Task(pr, "InputRegisterScriptTask")
  {

  }

  void
  InputRegisterScriptTask::execute
  ()
  {
    LOG_TRACE("InputRegisterScriptTask: Executing {}",getID());
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto inputComp = prLock->getInputComponent().lock())
      {
        if (inputComp->registerInputScript())
        {
          setState(TaskState::TASK_STATE_COMPLETED);
        }
      }
    }
    setState(TaskState::TASK_STATE_FAILED);
  }

  // InputExecuteScriptTask ==================================================

  InputExecuteScriptTask::InputExecuteScriptTask
  (const shared_ptr<ProjectRuntime>& pr)
    : Task(pr, "InputExecuteScriptTask")
  {
  }

  void
  InputExecuteScriptTask::execute
  ()
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto inputComp = prLock->getInputComponent().lock())
      {
        LOG_TRACE("InputExecuteScriptTask: Executing {}",getID());

        if (inputComp->executeInputScript())
        {
          setState(TaskState::TASK_STATE_COMPLETED);
        }
      }
    }
    setState(TaskState::TASK_STATE_FAILED);
  }

  // InputRemoveScriptTask ===================================================

  InputRemoveScriptTask::InputRemoveScriptTask
  (const shared_ptr<ProjectRuntime>& pr)
    : DestructionTask(pr, "InputRemoveScriptTask")
  {

  }

  void
  InputRemoveScriptTask::execute
  ()
  {
    LOG_TRACE("InputRemoveScriptTask: Executing {}",getID());
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto inputComp = prLock->getInputComponent().lock())
      {
        if (inputComp->removeInputScript(mInputScriptUuid))
        {
          setState(TaskState::TASK_STATE_COMPLETED);
        }
      }
    }
    setState(TaskState::TASK_STATE_FAILED);
  }

  void InputRemoveScriptTask::setInputScriptUuid(UuidType u)
  {
    mInputScriptUuid = u;
  }
}
