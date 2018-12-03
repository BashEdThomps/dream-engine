#include "FlyingInputHandler.h"
#include "InputComponent.h"
#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../Physics/PhysicsObjectInstance.h"

namespace Dream
{
    FlyingInputHandler::FlyingInputHandler
    (SceneRuntime* sRunt)
        : InputHandler (sRunt)
    {
        setLogClassName("DefaultInputHandler");
    }

    FlyingInputHandler::~FlyingInputHandler
    ()
    {

    }

    void
    FlyingInputHandler::onInput
    (InputComponent* ic)
    {
        auto cam = mSceneRuntime->getCamera();

        if (ic->isKeyDown(KeyboardMapping::KEY_W))
        {
            cam->flyForward();
        }
        if (ic->isKeyDown(KeyboardMapping::KEY_S))
        {
            cam->flyBackward();
        }

        static float pitchScalar = 0.05f;
        if (ic->isKeyDown(KeyboardMapping::KEY_A))
        {
            cam->flyLeft(pitchScalar);
        }
        if (ic->isKeyDown(KeyboardMapping::KEY_D))
        {
            cam->flyRight(pitchScalar);
        }

        if (ic->isKeyDown(KeyboardMapping::KEY_Q))
        {
            cam->flyDown();
        }
        if (ic->isKeyDown(KeyboardMapping::KEY_E))
        {
            cam->flyUp();
        }

        auto jsState = ic->getJoystickState();
        auto jsMapping = ic->getJoystickMapping();

        auto leftX = jsState.AxisData[jsMapping.AnalogLeftXAxis];
        auto leftY = jsState.AxisData[jsMapping.AnalogLeftYAxis];
        auto rightX = jsState.AxisData[jsMapping.AnalogRightXAxis];
        auto rightY = jsState.AxisData[jsMapping.AnalogRightYAxis];

        auto dPadUp = jsState.ButtonData[jsMapping.DPadNorth];
        auto dPadDown = jsState.ButtonData[jsMapping.DPadSouth];
        auto dPadLeft = jsState.ButtonData[jsMapping.DPadWest];
        auto dPadRight = jsState.ButtonData[jsMapping.DPadEast];

        auto lTrigger = jsState.ButtonData[jsMapping.TriggerLeftButton];
        auto rTrigger = jsState.ButtonData[jsMapping.TriggerRightButton];
        auto lShoulder = jsState.ButtonData[jsMapping.ShoulderLeft];
        auto rShoulder = jsState.ButtonData[jsMapping.ShoulderRight];

        static float jsSpeedScale = 0.075f;
        static float jsCamScale = 0.15f;
        cam->flyUp(ic->clearDeadzone(rightY)*jsCamScale);
        cam->flyLeft(ic->clearDeadzone(rightX)*jsCamScale);

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
            auto lx = ic->clearDeadzone(leftX);
            auto ly = ic->clearDeadzone(leftY);

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
                    po->setCenterOfMassTransform(originalTx*rotMat);
                    auto lv = po->getLinearVelocity().y;
                    po->setLinearVelocity(0.0f,0.0f,0.0f);
                    po->setAngularVelocity(0.0f,0.0f,0.0f);
                    po->clearForces();
                    mat4 newTransform = originalTx*rotMat*jsTx;
                    //po->setWorldTransform(newTransform);
                    vec4 newLv = newTransform[3] - originalTx[3];
                    if (yDelta == 0.0f)
                    {
                        newLv.y = lv;
                    }
                    else
                    {
                        newLv.y = yDelta*10;
                    }
                    po->setLinearVelocity(newLv.x*25.0f,newLv.y,newLv.z*25.0f);
                }
                else
                {
                    so->getTransform().setMatrix(originalTx*rotMat*jsTx);
                }
            }
        }
    }
}
