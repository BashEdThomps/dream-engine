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
using std::static_pointer_cast;

namespace octronic::dream
{
  InputComponent::InputComponent
  (const shared_ptr<ProjectRuntime>& rt)
    : Component (rt),
      mJoystickMapping(make_shared<JoystickMapping>(JsPsxMapping)),
      mKeyboardState(make_shared<KeyboardState>()),
      mMouseState(make_shared<MouseState>()),
      mJoystickState(make_shared<JoystickState>()),
      mPollDataTask(make_shared<InputPollDataTask>(rt)),
      mRegisterScriptTask(make_shared<InputRegisterScriptTask>(rt)),
      mExecuteScriptTask(make_shared<InputExecuteScriptTask>(rt)),
      mRemoveScriptTask(make_shared<InputRemoveScriptTask>(rt)),
      mJoystickCount(0)
  {
    LOG_TRACE("InputComponent: Constructing");
    //mJoystickNavigation = make_shared<Joystick2DPlaneNavigation>(&mJoystickState,&mJoystickMapping);
    mJoystickNavigation = make_shared<JoystickFaceForwardNavigation>(mJoystickState,mJoystickMapping);
  }

  InputComponent::~InputComponent
  ()
  {
    LOG_TRACE("InputComponent: Destructing");
  }

  bool
  InputComponent::init
  ()
  {
    LOG_DEBUG("InputComponent: Initialising...");
    return true;
  }

  // Keyboard ================================================================

  shared_ptr<KeyboardState>
  InputComponent::getKeyboardState
  ()
  const
  {
    return mKeyboardState;
  }

  void
  InputComponent::setKeyboardState
  (const shared_ptr<KeyboardState>& keyboardState)
  {
    mKeyboardState = keyboardState;
  }

  // Mouse ===================================================================

  shared_ptr<MouseState>
  InputComponent::getMouseState
  ()
  const
  {
    return mMouseState;
  }

  void
  InputComponent::setMouseState
  (const shared_ptr<MouseState>& mouseState)
  {
    mMouseState = mouseState;
  }

  // Joystick ================================================================

  shared_ptr<JoystickState>
  InputComponent::getJoystickState
  ()
  const
  {
    return mJoystickState;
  }

  void
  InputComponent::setJoystickState
  (const shared_ptr<JoystickState>& joystickState)
  {
    mJoystickState = joystickState;
  }

  shared_ptr<JoystickMapping>
  InputComponent::getJoystickMapping
  ()
  const
  {
    return mJoystickMapping;
  }

  shared_ptr<JoystickNavigation>
  InputComponent::getJoystickNavigation
  ()
  const
  {
    return mJoystickNavigation;
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
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto activeScene = prLock->getActiveSceneRuntime().lock())
      {
        mJoystickNavigation->update(activeScene);
        if (auto inputScript = activeScene->getInputScript().lock())
        {
          if (inputScript->getLoaded())
          {
            return inputScript->executeOnInput(activeScene);
          }
        }
      }
    }
    return true;
  }

  bool
  InputComponent::registerInputScript
  ()
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto activeScene = prLock->getActiveSceneRuntime().lock())
      {
        if (auto inputScript = activeScene->getInputScript().lock())
        {
          if (auto scriptComp = prLock->getScriptComponent().lock())
          {
          	return scriptComp->registerInputScript(inputScript);
          }
        }
      }
    }
    return false;
  }

  bool
  InputComponent::removeInputScript
  (UuidType inputScript)
  {
    if (inputScript != Uuid::INVALID)
    {
      if (auto prLock = mProjectRuntime.lock())
      {
        if (auto scriptComp = prLock->getScriptComponent().lock())
        {
          mRegisterScriptTask->setState(TASK_STATE_QUEUED);
          return scriptComp->removeInputScript(inputScript);
        }
      }
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
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto tqLock = prLock->getTaskQueue().lock())
      {
        tqLock->pushTask(mPollDataTask);

        if (auto activeScene = prLock->getActiveSceneRuntime().lock())
        {
          if (auto inputScript = activeScene->getInputScript().lock())
          {
            if (auto lfdTask = inputScript->getLoadFromDefinitionTask().lock())
            {

              if (lfdTask->hasState(TASK_STATE_COMPLETED) && inputScript->getLoaded())
              {
                mRemoveScriptTask->setInputScriptUuid(inputScript->getUuid());

                if (mRegisterScriptTask->hasState(TASK_STATE_QUEUED))
                {
                  tqLock->pushTask(mRegisterScriptTask);
                }
                else if (mRegisterScriptTask->hasState(TASK_STATE_COMPLETED))
                {
                  tqLock->pushTask(mExecuteScriptTask);
                }
              }
            }
          }
        }
      }
    }
  }
}
