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


#pragma once

#include "../Component.h"
#include "InputMapping.h"
#include "InputState.h"
#include "InputTasks.h"
#include <glm/fwd.hpp>
#include "../../Common/Math.h"

using glm::mat4;
using glm::quat;

namespace Dream
{

    class JoystickNavigation3D
    {
        float mLeftVelocity;
        float mRightVelocity;
        float mLeftTheta;
        float mRightTheta;
        Vector2 mHeading;

    public:
        JoystickNavigation3D()
            : mLeftVelocity(0.0f),
              mRightVelocity(0.0f),
              mLeftTheta(0.0f),
              mRightTheta(0.0f)
        {

        }

        JoystickNavigation3D
        (float leftX, float leftY, float rightX, float rightY)
        {
            mLeftVelocity = sqrt((leftX*leftX)+(-leftY * -leftY));
            mLeftTheta = atan2(-leftY,leftX);
            if (mLeftTheta != 0.0f)
            {
               mLeftTheta -= (M_PI/2);
            }

            mRightVelocity = sqrt((rightX * rightX) + (-rightY * -rightY));
            mRightTheta = atan2(-rightY,rightX);
            if (mRightTheta != 0.0f)
            {
                    mRightTheta -= (M_PI/2);
            }
            //if (mRightTheta < 0.0f) mRightTheta;
        }

        void show()
        {
            cout << "JSNav3D ="
                   " Lv: " << mLeftVelocity
                << " Lt: " << mLeftTheta
                << " Rv: " << mRightVelocity
                << " Rt: " << mRightTheta
                << endl;
        }

        float getRightVelocity() const
        {
            return mRightVelocity;
        }

        float getLeftTheta() const
        {
            return mLeftTheta;
        }

        float getRightTheta() const
        {
            return mRightTheta;
        }

        float getLeftVelocity() const
        {
            return mLeftVelocity;
        }

        void setHeading(const Vector2& h)
        {
            mHeading = h;
        }

        Vector2& getHeading()
        {
            return mHeading;
        }
    };

    class InputComponent : public Component
    {
        SceneRuntime* mCurrentSceneRuntime;

        KeyboardState mKeyboardState;
        KeyboardState mLastKeyboardState;

        MouseState mMouseState;
        MouseState mLastMouseState;

        JoystickState mJoystickState;
        JoystickState mLastJoystickState;
        JoystickMapping mJoystickMapping;
        InputPollDataTask mPollDataTask;
        InputExecuteScriptTask mExecuteScriptTask;
        JoystickNavigation3D mJoystickNavigation3D;

    public:
        InputComponent(ProjectRuntime* rt);
        ~InputComponent() override;

        bool init() override;
        bool executeInputScript();
        void pollData();

        void debugKeyboard() const;
        void debugMouse() const;
        void debugState() const;

        bool isKeyDown(int key);

        KeyboardState& getKeyboardState();
        void setKeyboardState(const KeyboardState& keyboardState);

        MouseState& getMouseState();
        void setMouseState(const MouseState& mouseState);

        JoystickMapping& getJoystickMapping();
        JoystickNavigation3D& getJoystickNavigation3D();
        JoystickState& getJoystickState();
        void setJoystickState(const JoystickState& joystickState);

        int getInputModeFlags() const;
        void setInputModeFlags(const int inputMode);

        float outsizeDeadzone(int axis);
        float mouseDeltaX();
        float mouseDeltaY();

        float clearDeadzone(float val);

        InputPollDataTask* getPollDataTask();
        InputExecuteScriptTask* getExecuteScriptTask();

        SceneRuntime *getCurrentSceneRuntime() const;
        void setCurrentSceneRuntime(SceneRuntime *currentSceneRuntime);
    };
}
