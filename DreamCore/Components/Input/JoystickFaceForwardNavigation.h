#pragma once

#include "JoystickNavigation.h"

namespace octronic::dream
{
	class SceneRuntime;
	class JoystickState;
	class JoystickMapping;

	class JoystickFaceForwardNavigation : public JoystickNavigation
    {
    public:
        JoystickFaceForwardNavigation(JoystickState* state, JoystickMapping* mapping);
        ~JoystickFaceForwardNavigation();
        void update(SceneRuntime* rt) override;
    };
}
