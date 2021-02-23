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

#include "Camera.h"

#include "Common/Logger.h"
#include "Common/Constants.h"
#include "Components/Window/WindowComponent.h"
#include "Project/ProjectRuntime.h"
#include "Scene/SceneRuntime.h"
#include "Entity/EntityRuntime.h"

#include <glm/common.hpp>

using glm::perspective;

namespace octronic::dream
{
    Camera::Camera
    (SceneRuntime* parent)
        : mProjectionMatrix(mat4(1.0f)),
          mFrustum(Frustum(this)),
          mFieldOfView(90.f),
          mSceneRuntime(parent)
    {
    }

    Camera::~Camera
    ()
    {
        LOG_DEBUG("Camera: Destroying Object");
    }

    mat4
    Camera::getViewMatrix
    ()
    const
    {
        return mTransform.getMatrix();
    }

    void
    Camera::update
    ()
    {
        auto wc = mSceneRuntime->getProjectRuntime()->getWindowComponent();
        float windowWidth  = (float)wc->getWidth();
        float windowHeight = (float)wc->getHeight();
        if (windowWidth == 0.f || windowHeight == 0.f) return;

        mProjectionMatrix = perspective(mFieldOfView,
                                        windowWidth/windowHeight,
                                        mSceneRuntime->getMinDrawDistance(),
                                        mSceneRuntime->getMaxDrawDistance());
        mFrustum.updatePlanes();
    }

    Transform
    Camera::getTransform
    ()
    const
    {
        return mTransform;
    }

    void
    Camera::setTransform
    (const Transform& translation)
    {
        mTransform = translation;
    }

    bool
    Camera::containedInFrustum
    (const EntityRuntime* sor)
    const
    {
        return mFrustum.testIntersection(sor->getTransform().getMatrix(),
                                         sor->getBoundingBox()) == Frustum::TEST_INSIDE;
    }

    bool
    Camera::containedInFrustum
    (const BoundingBox& bb)
    const
    {
        const static mat4 tx(1.0f);
        return mFrustum.testIntersection(tx,bb) == Frustum::TEST_INSIDE;
    }

    bool
    Camera::exceedsFrustumPlaneAtTranslation
    (Frustum::Plane plane, const EntityRuntime* sor, const vec3& tx)
    const
    {
        auto result = mFrustum.testIntersectionWithPlane(plane,tx,sor->getBoundingBox());
        return result != Frustum::TEST_INSIDE;
    }

    bool
    Camera::containedInFrustumAfterTransform
    (const EntityRuntime* sor, const mat4& tx)
    const
    {
        return mFrustum.testIntersection(sor->getTransform().getMatrix() * tx,
                                         sor->getBoundingBox()) != Frustum::TEST_OUTSIDE;
    }

    bool
    Camera::visibleInFrustum
    (const EntityRuntime* sor)
    const
    {
        return mFrustum.testIntersection(sor->getTransform().getMatrix(),
                                         sor->getBoundingBox()) != Frustum::TEST_OUTSIDE;
    }

    bool
    Camera::visibleInFrustum
    (const BoundingBox& bb)
    const
    {
        const static mat4 tx(1.0f);
        return mFrustum.testIntersection(tx,bb) != Frustum::TEST_OUTSIDE;
    }

    bool
    Camera::visibleInFrustum
    (const BoundingBox& bb, const mat4& tx)
    const
    {
        return mFrustum.testIntersection(tx,bb) != Frustum::TEST_OUTSIDE;
    }

    mat4
    Camera::getProjectionMatrix
    () const
    {
        return mProjectionMatrix;
    }

    void
    Camera::setProjectionMatrix
    (const mat4& projectionMatrix)
    {
        mProjectionMatrix = projectionMatrix;
    }

    void Camera::setFieldOfView(float fov)
    {
        mFieldOfView=fov;
    }

    float Camera::getFieldOfView() const
    {
        return mFieldOfView;
    }
}
