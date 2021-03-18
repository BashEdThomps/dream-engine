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
        JoystickFaceForwardNavigation(
                const weak_ptr<JoystickState>& state,
                const weak_ptr<JoystickMapping>& mapping);
        ~JoystickFaceForwardNavigation();

        void update(const weak_ptr<SceneRuntime>& rt) override;
    };
}
