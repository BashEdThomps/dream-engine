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
        // Eular Angles
        float mYaw;
        float mPitch;
        // Camera options
        float mMovementSpeed;
        float mMouseSensitivity;
        float mZoom;
        mat4 mProjectionMatrix;
        Frustum mFrustum;
        // Focus on SO
        SceneObjectRuntime* mFocusedSceneObject;
        float mFocusPitch;
        float mFocusYaw;
        float mFocusRadius;
        float mFocusElevation;
        vec3 mFocusTranslation;
    public:
        // Constructor with vectors
        Camera
        (
            vec3  position = vec3(0.0f, 0.0f, 0.0f),
            vec3  up       = vec3(0.0f, 1.0f, 0.0f),
            float yaw      = 0,
            float pitch    = 0
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

        void flyForward(float scalar=1.0f);
        void flyBackward(float scalar=1.0f);
        void flyLeft(float scalar=1.0f);
        void flyRight(float scalar=1.0f);
        void flyUp(float scalar=1.0f);
        void flyDown(float scalar=1.0f);

        void deltaPitch(float pitch);
        void deltaYaw(float yaw);

        float getFocusedObjectTheta();

        vec3 getUp();
        vec3 getFront();

        float getYaw() const;
        void setYaw(float yaw);

        float getPitch() const;
        void setPitch(float pitch);

        bool inFrustum(SceneObjectRuntime*);

        mat4 getProjectionMatrix() const;
        void setProjectionMatrix(const mat4& projectionMatrix);

        void setFocusedSceneObejct(SceneObjectRuntime*);
        SceneObjectRuntime* getFocusedSceneObject() const;

        float getFocusPitch() const;
        void setFocusPitch(float focusPitch);

        float getFocusYaw() const;
        void setFocusYaw(float focusYaw);

        float getFocusRadius() const;
        void setFocusRadius(float focusRadius);

        float getFocusElevation() const;
        void setFocusElevation(float focusElevation);

    private:
        void setFocusTranslationFromTarget(vec3 target);
    }; // End of Camera
} // End of Dream
