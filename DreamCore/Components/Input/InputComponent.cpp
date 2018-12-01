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
#include "../../Scene/SceneRuntime.h"
#include "../Graphics/Camera.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../Physics/PhysicsObjectInstance.h"

namespace Dream
{
    InputComponent::InputComponent
    (bool useKeyboard, bool useMouse, bool useJoystick)
        : IComponent (),
          mInputModeFlags(
              InputModeFlags::KeysOrbitCamera |
              InputModeFlags::JoystickFollowsObject |
              InputModeFlags::MouseFollowsObject
              ),
          mUseKeyboard(useKeyboard),
          mUseMouse(useMouse),
          mUseJoystick(useJoystick),
          mJoystickMapping(JsPsxMapping)
    {
        setLogClassName("InputComponent");
        auto log = getLog();
        log->trace("Constructing");
    }

    InputComponent::~InputComponent
    ()
    {
        auto log = getLog();
        log->trace("Destructing");
    }

    bool
    InputComponent::init
    ()
    {
        auto log = getLog();
        log->debug("Initialising...");


        if (mUseKeyboard)
        {
            log->debug("Creating Keyboard Device");
        }

        if (mUseMouse)
        {
            log->debug("Creating Mouse Device");
        }

        if (mUseJoystick)
        {
            log->debug("Creating Joystick Device");

        }

        return true;
    }

    void
    InputComponent::updateComponent
    (SceneRuntime* sRunt)
    {
        auto log = getLog();
        beginUpdate();
        mLastKeyboardState = mKeyboardState;
        mLastMouseState = mMouseState;
        mLastJoystickState = mJoystickState;

        auto cam = sRunt->getCamera();

        if ((mInputModeFlags & KeysOrbitCamera) == KeysOrbitCamera )
        {
            if (isKeyDown(KeyboardMapping::KEY_W))
            {
                cam->flyForward();
            }
            if (isKeyDown(KeyboardMapping::KEY_S))
            {
                cam->flyBackward();
            }

            static float pitchScalar = 0.05f;
            if (isKeyDown(KeyboardMapping::KEY_A))
            {
                cam->flyLeft(pitchScalar);
            }
            if (isKeyDown(KeyboardMapping::KEY_D))
            {
                cam->flyRight(pitchScalar);
            }

            if (isKeyDown(KeyboardMapping::KEY_Q))
            {
                cam->flyDown();
            }
            if (isKeyDown(KeyboardMapping::KEY_E))
            {
                cam->flyUp();
            }
        }

        if ((mInputModeFlags & JoystickFollowsObject) == JoystickFollowsObject)
        {
            auto leftX = mJoystickState.AxisData[mJoystickMapping.AnalogLeftXAxis];
            auto leftY = mJoystickState.AxisData[mJoystickMapping.AnalogLeftYAxis];
            auto rightX = mJoystickState.AxisData[mJoystickMapping.AnalogRightXAxis];
            auto rightY = mJoystickState.AxisData[mJoystickMapping.AnalogRightYAxis];

            auto dPadUp = mJoystickState.ButtonData[mJoystickMapping.DPadNorth];
            auto dPadDown = mJoystickState.ButtonData[mJoystickMapping.DPadSouth];
            auto dPadLeft = mJoystickState.ButtonData[mJoystickMapping.DPadWest];
            auto dPadRight = mJoystickState.ButtonData[mJoystickMapping.DPadEast];

            auto lTrigger = mJoystickState.ButtonData[mJoystickMapping.TriggerLeftButton];
            auto rTrigger = mJoystickState.ButtonData[mJoystickMapping.TriggerRightButton];
            auto lShoulder = mJoystickState.ButtonData[mJoystickMapping.ShoulderLeft];
            auto rShoulder = mJoystickState.ButtonData[mJoystickMapping.ShoulderRight];

            static float jsSpeedScale = 0.075f;
            static float jsCamScale = 0.15f;
            cam->flyUp(clearDeadzone(rightY)*jsCamScale);
            cam->flyLeft(clearDeadzone(rightX)*jsCamScale);

            if (dPadDown)
            {
                cam->flyBackward();
            }
            if (dPadUp)
            {
                cam->flyForward();
            }

            auto so = cam->getFocusedSceneObject();

            if (so)
            {
                auto lx = clearDeadzone(leftX);
                auto ly = clearDeadzone(leftY);

                float yDelta=0.0f;
                if (rTrigger)
                {
                    yDelta=jsSpeedScale;
                }
                else if (lTrigger)
                {
                    yDelta=-jsSpeedScale;
                }

                if (lx != 0.0f || ly != 0.0f || yDelta != 0.0f)
                {
                    auto decomp = so->getTransform().decomposeMatrix();
                    float distance = 0.0f;
                    float rot = 0.0f;
                    float distanceScaled = 0.0f;
                    bool rotationChanged = false;
                    mat4 originalTx = glm::translate(mat4(1.0f),decomp.translation);
                    mat4 jsTx(1.0f);
                    mat4 rotMat(1.0f);

                    // Theta of camera and joystick
                    if (lx != 0.0f || ly != 0.0f)
                    {
                        float xTheta = atan2(ly,lx);
                        float camTheta = cam->getFocusedObjectTheta();
                        rot = camTheta-(xTheta+(static_cast<float>(M_PI)/2.0f));
                        distance = sqrt((lx*lx)+(ly*ly));
                        distanceScaled = distance * jsSpeedScale;
                        rotationChanged = true;
                    }

                    if (rotationChanged)
                    {
                        rotMat = glm::rotate(mat4(1.0f),rot,vec3(0.0f,1.0f,0.0f));
                    }
                    else
                    {
                        rotMat = mat4_cast(decomp.rotation);
                    }

                    jsTx = glm::translate(mat4(1.0f), vec3(0.0f, 0.0f,-distanceScaled));


                    if (so->hasPhysicsObjectInstance())
                    {
                        auto po = so->getPhysicsObjectInstance();
                        originalTx = glm::translate(mat4(1.0f), po->getCenterOfMassPosition());
                        auto lv = po->getLinearVelocity().y;
                        po->setLinearVelocity(0.0f,0.0f,0.0f);
                        po->setAngularVelocity(0.0f,0.0f,0.0f);
                        po->clearForces();
                        po->setWorldTransform(originalTx*rotMat*jsTx);
                        if (yDelta == 0.0f)
                        {
                            po->setLinearVelocity(0.0f,lv,0.0f);
                        }
                        else
                        {
                            po->setLinearVelocity(0.0f,yDelta*10,0.0f);
                        }
                    }
                    else
                    {
                        so->getTransform().setMatrix(originalTx*rotMat*jsTx);
                    }
                }
            }
        }
        endUpdate();
    }

    bool
    InputComponent::usingKeyboard
    ()
    const
    {
        return mUseKeyboard;
    }

    bool
    InputComponent::usingMouse
    ()
    const
    {
        return mUseMouse;
    }

    bool
    InputComponent::usingJoystick
    ()
    const
    {
        return mUseJoystick;
    }

    bool
    InputComponent::isKeyDown
    (int key)
    {
        return mKeyboardState.KeysDown[key];
    }

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

    int
    InputComponent::getInputModeFlags
    ()
    const
    {
        return mInputModeFlags;
    }

    void
    InputComponent::setInputModeFlags
    (const int inputMode)
    {
        mInputModeFlags = inputMode;
    }

    float
    InputComponent::mouseDeltaX
    ()
    {
        return mMouseState.PosX - mLastMouseState.PosX;
    }

    float
    InputComponent::mouseDeltaY
    ()
    {
        return mMouseState.PosY - mLastMouseState.PosY;
    }

    float
    InputComponent::clearDeadzone
    (float val)
    {
        if(val > mJoystickState.DeadZone)
        {
            return  val - mJoystickState.DeadZone;
        }
        else if (val < -mJoystickState.DeadZone)
        {
            return val+mJoystickState.DeadZone;
        }
        return 0.0f;
    }

    const JoystickMapping
    InputComponent::JsPsxMapping
    {
        // Axis

        0,    // AnalogLeftXAxis
        1,    // AnalogLeftYAxis
        2,    // AnalogRightXAxis
        3,    // AnalogRightYAxis

        // Buttons

        12,   // FaceButtonNorth
        13,   // FaceButtonEast
        14,   // FaceButtonSouth
        15,   // FaceButtonWest

        0,   // FaceButtonSelect
        3,   // FaceButtonStart
        16,   // FaceButtonHome

        10,   // ShoulderLeft
        11,   // ShoulderRight

        8,   // LeftTriggerButton
        9,   // RighTriggerButton



        1,   // AnalogLeftButton
        2,   // AnalogRightButton

        4,   // DPadNorth
        5,   // DPadWest
        6,   // DPadSouth
        7    // DPadEast
    };

    const JoystickMapping
    InputComponent::JsXboxMapping
    {
        // Axis

        0, // AnalogLeftXAxis
        1, // AnalogLeftYAxis
        2, // AnalogRightXAxis
        3, // AnalogRightYAxis

        // Buttons

        0, // Y
        1, // B
        2, // A
        3, // X

        8,  // FaceButtonSelect
        9,  // FaceButtonStart
        12, // FaceButtonHome

        4, // ShoulderLeft
        5, // ShoulderRight

        6,  // LeftTriggerButton
        7,  // RighTriggerButton

        10, // AnalogLeftButton
        11, // AnalogRightButton

        13, // DPadNorth
        14, // DPadWest
        15, // DPadSouth
        16  // DPadEast
    };
}
