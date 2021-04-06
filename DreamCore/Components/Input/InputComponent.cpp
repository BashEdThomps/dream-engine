/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "InputComponent.h"
#include "InputTasks.h"
#include "JoystickMappings.h"
#include "JoystickFaceForwardNavigation.h"
#include "KeyboardMapping.h"
#include "Common/Logger.h"
#include "Components/Time.h"
#include "Math/Transform.h"
#include "Components/Script/ScriptRuntime.h"
#include "Components/Graphics/CameraRuntime.h"
#include "Scene/SceneRuntime.h"
#include "Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"

using std::make_shared;
using std::make_unique;
using std::static_pointer_cast;

namespace octronic::dream
{
  InputComponent::InputComponent
  (ProjectRuntime& rt)
    : Component (rt),
      mJoystickCount(0),
      mJoystickMapping(JsPsxMapping)
  {
    LOG_TRACE("InputComponent: Constructing");
  }

  bool
  InputComponent::init
  ()
  {
    LOG_DEBUG("InputComponent: Initialising...");
    //mJoystickNavigation = make_shared<Joystick2DPlaneNavigation>(&mJoystickState,&mJoystickMapping);
    mJoystickNavigation = make_unique<JoystickFaceForwardNavigation>(mJoystickState,mJoystickMapping);
    mPollDataTask = make_shared<InputPollDataTask>(getProjectRuntime());
    mRegisterScriptTask = make_shared<InputRegisterScriptTask>(getProjectRuntime());
    mExecuteScriptTask = make_shared<InputExecuteScriptTask>(getProjectRuntime());
    mRemoveScriptTask = make_shared<InputRemoveScriptTask>(getProjectRuntime());
    return true;
  }

  // Keyboard ================================================================

  KeyboardState&
  InputComponent::getKeyboardState
  ()
  {
    return mKeyboardState;
  }

  void
  InputComponent::setKeyboardState
  (const KeyboardState& keyboardState)
  {
    mKeyboardState = keyboardState;
  }

  // Mouse ===================================================================

  MouseState&
  InputComponent::getMouseState
  ()
  {
    return mMouseState;
  }

  void
  InputComponent::setMouseState
  (const MouseState& mouseState)
  {
    mMouseState = mouseState;
  }

  // Joystick ================================================================

  JoystickState&
  InputComponent::getJoystickState
  ()
  {
    return mJoystickState;
  }

  void
  InputComponent::setJoystickState
  (const JoystickState& joystickState)
  {
    mJoystickState = joystickState;
  }

  JoystickMapping&
  InputComponent::getJoystickMapping
  ()
  {
    return mJoystickMapping;
  }

  JoystickNavigation*
  InputComponent::getJoystickNavigation
  ()
  {
    return mJoystickNavigation.get();
  }

  int
  InputComponent::getJoystickCount
  ()
  const
  {
    return mJoystickCount;
  }

  void
  InputComponent::setJoystickCount
  (int joystickCount)
  {
    mJoystickCount = joystickCount;
  }

  // Scripting ===============================================================

  bool
  InputComponent::executeInputScript
  ()
  {
    auto activeSceneOpt = getProjectRuntime().getActiveSceneRuntime();
    if (activeSceneOpt)
    {
      auto& activeScene = activeSceneOpt.value().get();
      auto& inputScript = activeScene.getInputScript();

      mJoystickNavigation->update(activeScene);

      if (inputScript.getLoaded())
      {
        return inputScript.executeOnInput(activeScene);
      }
    }
    return false;
  }

  bool
  InputComponent::registerInputScript
  ()
  {
    auto activeSceneOpt = getProjectRuntime().getActiveSceneRuntime();
    if (activeSceneOpt)
    {
      auto& activeScene = activeSceneOpt.value().get();
      auto& inputScript = activeScene.getInputScript();
      auto& scriptComp = getProjectRuntime().getScriptComponent();
      return scriptComp.registerInputScript(inputScript);
    }
    return false;
  }

  bool
  InputComponent::removeInputScript
  (UuidType inputScript)
  {
    if (inputScript != Uuid::INVALID)
    {
      auto& scriptComp = getProjectRuntime().getScriptComponent();
      mRegisterScriptTask->setState(TASK_STATE_QUEUED);
      return scriptComp.removeInputScript(inputScript);
    }
    return false;
  }

  // Tasks ===================================================================

  shared_ptr<InputRegisterScriptTask>
  InputComponent::getRegisterScriptTask
  ()
  const
  {
    return mRegisterScriptTask;
  }

  shared_ptr<InputRemoveScriptTask>
  InputComponent::getRemoveScriptTask
  ()
  const
  {
    return mRemoveScriptTask;
  }

  void
  InputComponent::pollData
  ()
  {
  }

  void
  InputComponent::pushTasks()
  {
    auto& tq = getProjectRuntime().getTaskQueue();
    tq.pushTask(mPollDataTask);

    auto activeSceneOpt = getProjectRuntime().getActiveSceneRuntime();
    if (activeSceneOpt)
    {
      auto& activeScene = activeSceneOpt.value().get();
      auto& inputScript = activeScene.getInputScript();
      auto lfdTask = inputScript.getLoadFromDefinitionTask();

      if (lfdTask->hasState(TASK_STATE_COMPLETED) && inputScript.getLoaded())
      {
        if (mRegisterScriptTask->hasState(TASK_STATE_QUEUED))
        {
          tq.pushTask(mRegisterScriptTask);
          mRemoveScriptTask->setInputScriptUuid(inputScript.getUuid());
        }
        else if (mRegisterScriptTask->hasState(TASK_STATE_COMPLETED))
        {
          tq.pushTask(mExecuteScriptTask);
        }
      }
    }
  }
}
