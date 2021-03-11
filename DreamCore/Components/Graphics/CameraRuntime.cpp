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

#include "CameraRuntime.h"
#include "CameraDefinition.h"

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
    CameraRuntime::CameraRuntime
    (CameraDefinition* cDef, SceneRuntime* parent)
        : Runtime(cDef),
          mFreeTransform(true),
          mCameraEntityUuid(Uuid::INVALID),
          mCameraEntityRuntime(nullptr),
          mProjectionMatrix(mat4(1.f)),
          mSceneRuntime(parent),
          mUseEntity(false),
          mFieldOfView(90.f),
          mMinDrawDistance(0.1f),
          mMaxDrawDistance(1000.f),
          mMeshCullDistance(100.f),
          mFrustum(Frustum(this))
    {
    }

    CameraRuntime::~CameraRuntime
    ()
    {
        LOG_DEBUG("Camera: Destroying Object");
    }

    bool CameraRuntime::loadFromDefinition()
    {
        CameraDefinition* cDef = static_cast<CameraDefinition*>(mDefinitionHandle);
        mFreeTransform = cDef->getFreeTransform();
        mCameraEntityUuid = cDef->getCameraEntityUuid();
        mUseEntity = cDef->getUseEntity();
        mFieldOfView = cDef->getFieldOfView();
        mMinDrawDistance = cDef->getMinDrawDistance();
        mMaxDrawDistance = cDef->getMaxDrawDistance();
        mMeshCullDistance = cDef->getMeshCullDistance();
        return true;
    }

    mat4
    CameraRuntime::getViewMatrix
    ()
    const
    {
        if (mUseEntity)
        {
            if(mCameraEntityRuntime != nullptr)
            {
                return mCameraEntityRuntime->getTransform().getMatrix();
            }
            else
            {
                return mat4(1.f);
            }
        }
        else
        {
            return mFreeTransform.getMatrix();
        }
    }

    void
    CameraRuntime::update
    ()
    {
        if (mUseEntity)
        {
          	mCameraEntityRuntime = mSceneRuntime->getEntityRuntimeByUuid(mCameraEntityUuid);
        }

        auto wc = mSceneRuntime->getProjectRuntime()->getWindowComponent();
        float windowWidth  = (float)wc->getWidth();
        float windowHeight = (float)wc->getHeight();
        if (windowWidth == 0.f || windowHeight == 0.f) return;

        mProjectionMatrix = perspective(mFieldOfView, windowWidth/windowHeight,
                                        mMinDrawDistance, mMaxDrawDistance);
        mFrustum.updatePlanes();
    }

    bool
    CameraRuntime::containedInFrustum
    (const EntityRuntime* sor)
    const
    {
        return mFrustum.testIntersection(sor->getTransform().getMatrix(),
                                         sor->getBoundingBox()) == Frustum::TEST_INSIDE;
    }

    bool
    CameraRuntime::containedInFrustum
    (const BoundingBox& bb)
    const
    {
        const static mat4 tx(1.0f);
        return mFrustum.testIntersection(tx,bb) == Frustum::TEST_INSIDE;
    }

    bool
    CameraRuntime::exceedsFrustumPlaneAtTranslation
    (Frustum::Plane plane, const EntityRuntime* sor, const vec3& tx)
    const
    {
        auto result = mFrustum.testIntersectionWithPlane(plane,tx,sor->getBoundingBox());
        return result != Frustum::TEST_INSIDE;
    }

    bool
    CameraRuntime::containedInFrustumAfterTransform
    (const EntityRuntime* sor, const mat4& tx)
    const
    {
        return mFrustum.testIntersection(sor->getTransform().getMatrix() * tx,
                                         sor->getBoundingBox()) != Frustum::TEST_OUTSIDE;
    }

    bool
    CameraRuntime::visibleInFrustum
    (const EntityRuntime* sor)
    const
    {
        return mFrustum.testIntersection(sor->getTransform().getMatrix(),
                                         sor->getBoundingBox()) != Frustum::TEST_OUTSIDE;
    }

    bool
    CameraRuntime::visibleInFrustum
    (const BoundingBox& bb)
    const
    {
        const static mat4 tx(1.0f);
        return mFrustum.testIntersection(tx,bb) != Frustum::TEST_OUTSIDE;
    }

    bool
    CameraRuntime::visibleInFrustum
    (const BoundingBox& bb, const mat4& tx)
    const
    {
        return mFrustum.testIntersection(tx,bb) != Frustum::TEST_OUTSIDE;
    }

    mat4
    CameraRuntime::getProjectionMatrix
    () const
    {
        return mProjectionMatrix;
    }

    void
    CameraRuntime::setProjectionMatrix
    (const mat4& projectionMatrix)
    {
        mProjectionMatrix = projectionMatrix;
    }

    void CameraRuntime::setFieldOfView(float fov)
    {
        mFieldOfView = fov;
    }

    float CameraRuntime::getFieldOfView() const
    {
        return mFieldOfView;
    }

    void CameraRuntime::setCameraEntityRuntime(EntityRuntime *er)
    {
        mCameraEntityRuntime = er;
    }

    EntityRuntime* CameraRuntime::getCameraEntityRuntime() const
    {
        return mCameraEntityRuntime;
    }

    UuidType CameraRuntime::getCameraEntityUuid() const
    {
        return mCameraEntityUuid;
    }

    void CameraRuntime::setCameraEntityUuid(UuidType u)
    {
        mCameraEntityUuid = u;
    }

    void CameraRuntime::setTransform(const Transform& t)
    {
        if (mUseEntity)
        {
            if (mCameraEntityRuntime) mCameraEntityRuntime->setTransform(t);
        }
        else
        {
            mFreeTransform = t;
        }
    }

    Transform CameraRuntime::getTransform() const
    {
        if (mUseEntity)
        {
            if (mCameraEntityRuntime) return mCameraEntityRuntime->getTransform();
            else return Transform();
        }
        else
        {
            return mFreeTransform;
        }
    }

    void
    CameraRuntime::setMeshCullDistance
    (float mcd)
    {
        mMeshCullDistance = mcd;
    }

    float
    CameraRuntime::getMeshCullDistance
    ()
    const
    {
        return mMeshCullDistance;
    }

    void
    CameraRuntime::setMinDrawDistance
    (float f)
    {
        mMinDrawDistance = f;
    }

    float
    CameraRuntime::getMinDrawDistance
    ()
    const
    {
        return mMinDrawDistance;
    }

    float
    CameraRuntime::getMaxDrawDistance
    ()
    const
    {
        return mMaxDrawDistance;
    }

    void
    CameraRuntime::setMaxDrawDistance(float mdd)
    {
        mMaxDrawDistance = mdd;
    }

    void CameraRuntime::setUseEntity(bool ue)
    {
        mUseEntity = ue;
    }

    bool CameraRuntime::getUseEntity() const
    {
        return mUseEntity;
    }

    void CameraRuntime::captureDefinition()
    {
 		CameraDefinition* cDef = static_cast<CameraDefinition*>(mDefinitionHandle);
        cDef->setFreeTransform(mFreeTransform);
        cDef->setCameraEntityUuid(mCameraEntityUuid);
        cDef->setUseEntity(mUseEntity);
        cDef->setFieldOfView(mFieldOfView);
        cDef->setMinDrawDistance(mMinDrawDistance);
        cDef->setMaxDrawDistance(mMaxDrawDistance);
        cDef->setMeshCullDistance(mMeshCullDistance);
    }
}
