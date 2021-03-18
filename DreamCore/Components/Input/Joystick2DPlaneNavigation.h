#pragma once

#include "JoystickNavigation.h"

namespace octronic::dream
{
	class JoystickState;
	class JoystickMapping;

	class Joystick2DPlaneNavigation : public JoystickNavigation
    {
    public:
        Joystick2DPlaneNavigation(
                const weak_ptr<JoystickState>& state,
                const weak_ptr<JoystickMapping>& mapping);
        ~Joystick2DPlaneNavigation();

        void update(const weak_ptr<SceneRuntime>& rt) override;
        void show();
        float getRightVelocity() const;
        float getLeftTheta() const;
        float getRightTheta() const;
        float getLeftVelocity() const;
    private:
        float mLeftVelocity;
        float mRightVelocity;
        float mLeftTheta;
        float mRightTheta;

        float mLastLeftVelocity;
        float mLastRightVelocity;
        float mLastLeftTheta;
        float mLastRightTheta;
    };
}
