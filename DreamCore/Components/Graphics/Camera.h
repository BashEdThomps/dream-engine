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

#define GLM_FORCE_RADIANS

#ifdef WIN32
    #define _USE_MATH_DEFINES // for C++
#endif

#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../../Scene/SceneObject/BoundingBox.h"
#include "../../Common/DreamObject.h"
#include "../Transform.h"
#include "Frustum.h"

using glm::vec3;
using glm::mat4;

namespace Dream
{
    class SceneObjectRuntime;
    class Camera : public DreamObject
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
        mat4 mProjectionMatrix;
        Frustum mFrustum;
    public:
        // Constructor with vectors
        Camera
        (
            vec3  position = vec3(0.0f, 0.0f, 0.0f),
            vec3  up       = vec3(0.0f, 1.0f, 0.0f),
            float yaw      = 0,//Constants::CAMERA_YAW,
            float pitch    = 0//Constants::CAMERA_PITCH
        );

        // Constructor with scalar values
        ~Camera();
        mat4 getViewMatrix();
        void updateCameraVectors();
        void setTranslation(vec3);
        void setTranslation(float,float,float);
        vec3 getTranslation();
        void  setMovementSpeed(float);
        float getMovementSpeed();
        void setLookAt(float x, float y, float z);
        void setLookAt(vec3);
        vec3 getLookAt();
        void setFreeMode(bool freemode);
        void orbit(vec3,float elevation, float radius, float pitch, float yaw);

        void flyForward();
        void flyBackward();
        void flyLeft();
        void flyRight();
        void flyUp();
        void flyDown();
        void flyX();
        void flyY();
        void flyZ();

        void deltaPitch(float pitch);
        void deltaYaw(float yaw);

        vec3 getRelativeTranslation(float relative);
        mat4 getRelativeRotation(vec3 relative);

        vec3 getUp();
        vec3 getFront();
        float getYaw() const;
        void setYaw(float yaw);
        float getPitch() const;
        void setPitch(float pitch);
        bool inFrustum(SceneObjectRuntime*);
        mat4 getProjectionMatrix() const;
        void setProjectionMatrix(const mat4& projectionMatrix);
    }; // End of Camera
} // End of Dream
