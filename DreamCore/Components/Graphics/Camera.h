#pragma once

#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

#include "../../Common/Constants.h"
#include "../../Common/ILoggable.h"
#include "../Transform3D.h"

using glm::vec3;
using glm::mat4;

namespace Dream
{
    class Camera : public ILoggable
    {
    private:
        // Camera Attributes
        vec3 mTranslation;
        vec3 mFront;
        vec3 mUp;
        vec3 mRight;
        vec3 mWorldUp;
        vec3 mLookAt;
        bool mFreeMode;
        // Eular Angles
        float mYaw;
        float mPitch;
        // Camera options
        float mMovementSpeed;
        float mMouseSensitivity;
        float mZoom;
    public:
        // Constructor with vectors
        Camera
        (
            vec3  position = vec3(0.0f, 0.0f, 0.0f),
            vec3  up       = vec3(0.0f, 1.0f, 0.0f),
            float yaw      = Constants::CAMERA_YAW,
            float pitch    = Constants::CAMERA_PITCH
        );

        // Constructor with scalar values
        Camera(float, float, float, float, float, float, float, float);
        ~Camera();
        mat4 getViewMatrix();
        void processKeyboard(unsigned int, float);
        void processMouseMovement(float, float, bool);
        void pan(float, float, bool);
        void processMouseScroll(float);
        void updateCameraVectors();
        void setTranslation(vec3);
        void setTranslation(float,float,float);
        vec3 getTranslation();
        void setRotation(vec3);
        vec3 getRotation();
        void  setMovementSpeed(float);
        float getMovementSpeed();
        void  setMouseSensitivity(float);
        float getMouseSensitivity();
        float getZoom();
        void setLookAt(float x, float y, float z);
        void setLookAt(Transform3D);
        void setFreeMode(bool freemode);
        void setTransform(Transform3D);
        void orbit(Transform3D target,float elevation, float radius, float pitch, float yaw);

        void flyForward(float speed);
        void flyBackward(float speed);
        void flyLeft(float speed);
        void flyRight(float speed);
        void flyUp(float speed);
        void flyDown(float speed);
        void flyX(float speed);
        void flyY(float speed);
        void flyZ(float speed);

        vec3 getRelativeTranslation(float relative);
        mat4 getRelativeRotation(vec3 relative);

        vec3 getUp();
        vec3 getFront();

    private:
        float radians(float);

    }; // End of Camera

} // End of Dream
