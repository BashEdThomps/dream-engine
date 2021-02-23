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

#include "Math/Transform.h"
#include "Entity/BoundingBox.h"
#include "Frustum.h"

using glm::mat4;

namespace octronic::dream
{
    class SceneRuntime;
    class EntityRuntime;

    /**
     * @brief Implements a Camera in 3D Space.
     */
    class Camera
    {

    public:
        Camera(SceneRuntime* parent);
        ~Camera();

        mat4 getViewMatrix() const;
        void update();

        void setTransform(const Transform&);
        Transform getTransform() const;

        bool containedInFrustum(const EntityRuntime*) const;
        bool containedInFrustum(const BoundingBox&) const;
        bool containedInFrustumAfterTransform(const EntityRuntime*,const mat4& tx) const;
        bool exceedsFrustumPlaneAtTranslation(Frustum::Plane plane, const EntityRuntime*sor, const vec3& tx) const;

        bool visibleInFrustum(const EntityRuntime*)const;
        bool visibleInFrustum(const BoundingBox&) const;

        mat4 getProjectionMatrix() const;
        void setProjectionMatrix(const mat4& projectionMatrix);

        void updateProjectionMatrix(float w, float h);
        bool visibleInFrustum(const BoundingBox& bb,const mat4& tx) const;

        void setFieldOfView(float fov);
        float getFieldOfView() const;

    private:
        void setFocusTranslationFromTarget(const vec3& target);

    private:
        float mFieldOfView;
        Transform mTransform;
        mat4 mProjectionMatrix;
        Frustum mFrustum;
        SceneRuntime* mSceneRuntime;
    };
}
