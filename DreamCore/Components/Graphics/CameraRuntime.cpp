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
using std::static_pointer_cast;

namespace octronic::dream
{
    CameraRuntime::CameraRuntime
    (const weak_ptr<CameraDefinition>& cDef,
     const weak_ptr<SceneRuntime>& parent)
        : Runtime(cDef),
          mFreeTransform(true),
          mCameraEntityUuid(Uuid::INVALID),
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
        if (auto defLock = mDefinition.lock())
        {
            auto cDef = static_pointer_cast<CameraDefinition>(defLock);
            mFreeTransform = cDef->getFreeTransform();
            mCameraEntityUuid = cDef->getCameraEntityUuid();
            mUseEntity = cDef->getUseEntity();
            mFieldOfView = cDef->getFieldOfView();
            mMinDrawDistance = cDef->getMinDrawDistance();
            mMaxDrawDistance = cDef->getMaxDrawDistance();
            mMeshCullDistance = cDef->getMeshCullDistance();
            return true;
        }
        return false;
    }

    mat4
    CameraRuntime::getViewMatrix
    ()
    const
    {
        if (mUseEntity)
        {
            if(auto camLock = mCameraEntityRuntime.lock())
            {
                return camLock->getTransform().getMatrix();
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
        if (auto srLock = mSceneRuntime.lock())
        {
            if (mUseEntity)
            {
                mCameraEntityRuntime = srLock->getEntityRuntimeByUuid(mCameraEntityUuid);
            }

            if (auto prLock = srLock->getProjectRuntime().lock())
            {
                if (auto wcLock = prLock->getWindowComponent().lock())
                {

                    float windowWidth  = (float)wcLock->getWidth();
                    float windowHeight = (float)wcLock->getHeight();
                    if (windowWidth == 0.f || windowHeight == 0.f) return;

                    mProjectionMatrix = perspective(mFieldOfView, windowWidth/windowHeight,
                                                    mMinDrawDistance, mMaxDrawDistance);
                    mFrustum.updatePlanes();
                }
            }
        }
    }

    bool
    CameraRuntime::containedInFrustum
    (const weak_ptr<EntityRuntime>& er)
    const
    {
        if (auto erLock = er.lock())
        {
            return mFrustum.testIntersection(erLock->getTransform().getMatrix(),
                                             erLock->getBoundingBox()) == Frustum::TEST_INSIDE;
        }
        return false;
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
    (Frustum::Plane plane, const weak_ptr<EntityRuntime>& er, const vec3& tx)
    const
    {
        if (auto erLock = er.lock())
        {
            auto result = mFrustum.testIntersectionWithPlane(plane,tx,erLock->getBoundingBox());
            return result != Frustum::TEST_INSIDE;
        }
        return false;
    }

    bool
    CameraRuntime::containedInFrustumAfterTransform
    (const weak_ptr<EntityRuntime>& er, const mat4& tx)
    const
    {
        if (auto erLock = er.lock())
        {
            return mFrustum.testIntersection(erLock->getTransform().getMatrix() * tx,
                                             erLock->getBoundingBox()) != Frustum::TEST_OUTSIDE;
        }
        return false;
    }

    bool
    CameraRuntime::visibleInFrustum
    (const weak_ptr<EntityRuntime>& er)
    const
    {
        if (auto erLock = er.lock())
        {
            return mFrustum.testIntersection(erLock->getTransform().getMatrix(),
                                             erLock->getBoundingBox()) != Frustum::TEST_OUTSIDE;
        }
        return false;
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

    void
    CameraRuntime::setFieldOfView
    (float fov)
    {
        mFieldOfView = fov;
    }

    float
    CameraRuntime::getFieldOfView
    () const
    {
        return mFieldOfView;
    }

    void
    CameraRuntime::setCameraEntityRuntime
    (const weak_ptr<EntityRuntime>& er)
    {
        mCameraEntityRuntime = er;
    }

    weak_ptr<EntityRuntime>
    CameraRuntime::getCameraEntityRuntime
    () const
    {
        return mCameraEntityRuntime;
    }

    UuidType
    CameraRuntime::getCameraEntityUuid
    () const
    {
        return mCameraEntityUuid;
    }

    void
    CameraRuntime::setCameraEntityUuid
    (UuidType u)
    {
        mCameraEntityUuid = u;
    }

    void
    CameraRuntime::setTransform
    (const Transform& t)
    {
        if (mUseEntity)
        {
            if (auto ceLock = mCameraEntityRuntime.lock())
            {
                ceLock->setTransform(t);
            }
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
            if (auto ceLock = mCameraEntityRuntime.lock())
            {
                return ceLock->getTransform();
            }
            else
            {
                return Transform();
            }
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
        if (auto defLock = mDefinition.lock())
        {
            auto cDef = static_pointer_cast<CameraDefinition>(defLock);
            cDef->setFreeTransform(mFreeTransform);
            cDef->setCameraEntityUuid(mCameraEntityUuid);
            cDef->setUseEntity(mUseEntity);
            cDef->setFieldOfView(mFieldOfView);
            cDef->setMinDrawDistance(mMinDrawDistance);
            cDef->setMaxDrawDistance(mMaxDrawDistance);
            cDef->setMeshCullDistance(mMeshCullDistance);
        }
    }
}
