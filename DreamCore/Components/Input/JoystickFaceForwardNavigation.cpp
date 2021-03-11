#include "JoystickFaceForwardNavigation.h"

#include "JoystickState.h"
#include "JoystickMapping.h"

#include "Components/Time.h"
#include "Scene/SceneRuntime.h"
#include "Project/ProjectRuntime.h"

namespace octronic::dream
{
	JoystickFaceForwardNavigation::JoystickFaceForwardNavigation
    (JoystickState* state, JoystickMapping* mapping)
        : JoystickNavigation(state,mapping)
    {

    }

    JoystickFaceForwardNavigation::~JoystickFaceForwardNavigation()
    {

    }

    void JoystickFaceForwardNavigation::update(SceneRuntime* rt)
    {
        if (!rt) return;

        float leftX = mJoystickState->getAxisData(mJoystickMapping->AnalogLeftXAxis);
        float leftY = mJoystickState->getAxisData(mJoystickMapping->AnalogLeftYAxis);
        float rightX = mJoystickState->getAxisData(mJoystickMapping->AnalogRightXAxis);
        float rightY = mJoystickState->getAxisData(mJoystickMapping->AnalogRightYAxis);

		Transform playerTransform;
		Time* time = rt->getProjectRuntime()->getTime();
		CameraRuntime* camera = rt->getCamera();
		static mat4 ident(1.0f);
		vec3 translation = playerTransform.getTranslation();
		mat4 mtx = glm::translate(ident,translation);
		auto totalYaw = 0.0f;
		setHeading(vec2(cos(totalYaw),-sin(totalYaw)));
		mtx = glm::translate(mtx,vec3(time->perSecond(-leftY)*10.0f,0,time->perSecond(leftX)*10.0f));
    }
}
