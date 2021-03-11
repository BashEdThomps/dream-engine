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

namespace octronic::dream
{
    InputComponent::InputComponent
    (ProjectRuntime* rt)
        : Component (rt),
          mJoystickMapping(JsPsxMapping),
          mJoystickNavigation(nullptr),
          mPollDataTask(make_shared<InputPollDataTask>(rt)),
          mRegisterScriptTask(make_shared<InputRegisterScriptTask>(rt)),
          mExecuteScriptTask(make_shared<InputExecuteScriptTask>(rt)),
          mRemoveScriptTask(make_shared<InputRemoveScriptTask>(rt)),
          mJoystickCount(0)
    {
        LOG_TRACE("InputComponent: Constructing");
        //mJoystickNavigation = make_shared<Joystick2DPlaneNavigation>(&mJoystickState,&mJoystickMapping);
        mJoystickNavigation = make_shared<JoystickFaceForwardNavigation>(&mJoystickState,&mJoystickMapping);
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

    KeyboardState
    InputComponent::getKeyboardState
    ()
    const
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

    MouseState
    InputComponent::getMouseState
    ()
    const
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

    JoystickState
    InputComponent::getJoystickState
    ()
    const
    {
        return mJoystickState;
    }

    void
    InputComponent::setJoystickState
    (const JoystickState& joystickState)
    {
        mJoystickState = joystickState;
    }

    JoystickMapping
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
        SceneRuntime* activeScene = mProjectRuntime->getActiveSceneRuntime();

        if (activeScene != nullptr)
        {
            mJoystickNavigation->update(activeScene);
        	ScriptRuntime* inputScript = activeScene->getInputScript();
            if (inputScript != nullptr && inputScript->getLoaded())
            {
                return inputScript->executeOnInput(this, activeScene);
            }
        }
        return true;
    }

    bool
    InputComponent::registerInputScript
    ()
    {
        SceneRuntime* activeScene = mProjectRuntime->getActiveSceneRuntime();
        if (activeScene != nullptr)
        {
			ScriptRuntime* inputScript = activeScene->getInputScript();
			if (inputScript != nullptr)
			{
				auto scriptComp = mProjectRuntime->getScriptComponent();
				return scriptComp->registerInputScript(inputScript);
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
			auto scriptComp = mProjectRuntime->getScriptComponent();
            mRegisterScriptTask->setState(TASK_STATE_QUEUED);
			return scriptComp->removeInputScript(inputScript);
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
    	auto taskQueue = mProjectRuntime->getTaskQueue();
        SceneRuntime* activeScene = mProjectRuntime->getActiveSceneRuntime();

        taskQueue->pushTask(mPollDataTask);

        if (activeScene != nullptr)
        {
            ScriptRuntime* inputScript = activeScene->getInputScript();

			if (inputScript != nullptr)
			{
				auto lfdTask = inputScript->getLoadFromDefinitionTask();

				if (lfdTask->hasState(TASK_STATE_COMPLETED) && inputScript->getLoaded())
				{
                    mRemoveScriptTask->setInputScriptUuid(inputScript->getUuid());

					if (mRegisterScriptTask->hasState(TASK_STATE_QUEUED))
					{
						taskQueue->pushTask(mRegisterScriptTask);
					}
					else if (mRegisterScriptTask->hasState(TASK_STATE_COMPLETED))
					{
						taskQueue->pushTask(mExecuteScriptTask);
					}
				}
			}
        }
    }
}
