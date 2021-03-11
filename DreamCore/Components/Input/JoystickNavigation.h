#pragma once

#include "JoystickState.h"
#include "JoystickMapping.h"

#include <glm/vec2.hpp>

using glm::vec2;

namespace octronic::dream
{
	class SceneRuntime;
	class JoystickNavigation
    {
    public:
        JoystickNavigation(JoystickState* state, JoystickMapping* mapping);
        virtual ~JoystickNavigation();
        virtual void update(SceneRuntime* rt) = 0;
        void setHeading(const vec2& h);
        vec2 getHeading();
    protected:
        JoystickState* mJoystickState;
        JoystickMapping* mJoystickMapping;
        vec2 mHeading;
    };
}
