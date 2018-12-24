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

#include "../Window/WindowComponent.h"
#include "../../Project/ProjectRuntime.h"
#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"

namespace Dream
{
    Camera::Camera
    (SceneRuntime* parent)
        : DreamObject("Camera"),
          mSceneRuntime(parent),
          mTranslation(vec3(0.0f, 0.0f, 0.0f)),
          mFront(vec3(0.0f, 0.0f, -1.0f)),
          mWorldUp(vec3(0.0f, 1.0f, 0.0f)),
          mYaw(0.0f),
          mPitch(0.0f),
          mMovementSpeed(Constants::CAMERA_SPEED),
          mFrustum(Frustum(this)),
          mFocusedSceneObject(nullptr),
          mFocusPitch(0.0f),
          mFocusYaw(0.0f),
          mFocusRadius(10.0f),
          mFocusElevation(0.0f),
          mFocusTranslation(vec3(0.0f)),
          mProjectionMatrix(mat4(1.0f)),
          mMinimumDraw(1.0f),
          mMaximumDraw(3000.0f),
          mMeshCullDistance(2500.0f)
    {
    }

    Camera::~Camera
    ()
    {
        auto log = getLog();
        log->debug("Destroying Object");
        return;
    }

    float Camera::getYaw() const
    {
        return mYaw;
    }

    void Camera::setYaw(float yaw)
    {
        mYaw = yaw;
    }

    float Camera::getPitch() const
    {
        return mPitch;
    }

    void Camera::setPitch(float pitch)
    {
        mPitch = pitch;
    }

    mat4
    Camera::getViewMatrix
    ()
    {
        if (mFocusedSceneObject)
        {
            return lookAt(mFocusTranslation,mFocusedSceneObject->getTransform().decomposeMatrix().translation,mUp);
        }
        else
        {
            return lookAt(mTranslation,mTranslation+mFront, mUp);
        }
    }

    void
    Camera::update
    ()
    {
        mMeshCullDistance = mSceneRuntime->getMeshCullDistance();
        mMinimumDraw = mSceneRuntime->getMinDrawDistance();
        mMaximumDraw = mSceneRuntime->getMaxDrawDistance();
        auto wc = mSceneRuntime->getProjectRuntime()->getWindowComponent();
        int windowWidth  = wc->getWidth();
        int windowHeight = wc->getHeight();
        updateProjectionMatrix(static_cast<float>(windowWidth),static_cast<float>(windowHeight));
        updateCameraVectors();
    }

    void
    Camera::updateProjectionMatrix
    (float w, float h)
    {
       mProjectionMatrix = perspective(Constants::CAMERA_ZOOM, w/h,mMinimumDraw,mMaximumDraw);
    }

    vec3
    Camera::getUp
    ()
    {
        return mUp;
    }

    vec3
    Camera::getFront
    ()
    {
        return mFront;
    }

    void
    Camera::flyForward
    (float scalar)
    {
        if (mFocusedSceneObject)
        {
           mFocusRadius -= mMovementSpeed*scalar;
           if (mFocusRadius < 0.0f)
           {
               mFocusRadius = 0.1f;
           }
        }
        else
        {
            mTranslation.x += mFront.x * mMovementSpeed*scalar;
            mTranslation.y += mFront.y * mMovementSpeed*scalar;
            mTranslation.z += mFront.z * mMovementSpeed*scalar;
        }
    }

    void
    Camera::flyBackward
    (float scalar)
    {
        if (mFocusedSceneObject)
        {
           mFocusRadius += mMovementSpeed*scalar;
        }
        else
        {
            mTranslation.x -= mFront.x * mMovementSpeed*scalar;
            mTranslation.y -= mFront.y * mMovementSpeed*scalar;
            mTranslation.z -= mFront.z * mMovementSpeed*scalar;
        }
    }

    void
    Camera::flyLeft
    (float scalar)
    {
        if (mFocusedSceneObject)
        {
            mFocusYaw -= mMovementSpeed*scalar;
        }
        else
        {
            mTranslation.x -= mRight.x * mMovementSpeed*scalar;
            mTranslation.y -= mRight.y * mMovementSpeed*scalar;
            mTranslation.z -= mRight.z * mMovementSpeed*scalar;
        }
    }

    void
    Camera::flyRight
    (float scalar)
    {
        if (mFocusedSceneObject)
        {
            mFocusYaw += mMovementSpeed*scalar;
        }
        else
        {
            mTranslation.x += mRight.x * mMovementSpeed*scalar;
            mTranslation.y += mRight.y * mMovementSpeed*scalar;
            mTranslation.z += mRight.z * mMovementSpeed*scalar;
        }
    }

    void
    Camera::flyUp
    (float scalar)
    {
        if (mFocusedSceneObject)
        {
            mFocusElevation += mMovementSpeed*scalar;
        }
        else
        {
            mTranslation.y += mMovementSpeed*scalar;
        }
    }

    void
    Camera::flyDown
    (float scalar)
    {
        if (mFocusedSceneObject)
        {
            mFocusElevation -= mMovementSpeed*scalar;
        }
        else
        {
            mTranslation.y -= mMovementSpeed*scalar;
        }
    }

    vec3
    Camera::getTranslation
    ()
    {
        return mTranslation;
    }

    void
    Camera::updateCameraVectors
    ()
    {
        if (mFocusedSceneObject)
        {
            vec3 tx = mFocusedSceneObject->getTransform().decomposeMatrix().translation;
            setFocusTranslationFromTarget(tx);
            mFront = normalize(tx);
            mRight = normalize(cross(mFront, mWorldUp));
            mUp    = mWorldUp;
        }
        else
        {
            mFront.x = static_cast<float>(cos(mYaw) * cos(mPitch));
            mFront.y = static_cast<float>(sin(mPitch));
            mFront.z = static_cast<float>(sin(mYaw) * cos(mPitch));
            mFront = normalize(mFront);
            mRight = normalize(cross(mFront, mWorldUp));
            mUp    = mWorldUp;
        }

        mFrustum.updatePlanes();
    }

    void
    Camera::setTranslation
    (vec3 translation)
    {
        mTranslation = translation;
    }

    void Camera::setTranslation(float x, float y , float z)
    {
        mTranslation.x = x;
        mTranslation.y = y;
        mTranslation.z = z;
    }

    void
    Camera::setMovementSpeed
    (float movementSpeed)
    {
        mMovementSpeed = movementSpeed;
    }

    float
    Camera::getMovementSpeed
    ()
    {
        return mMovementSpeed;
    }

    void
    Camera::setFocusTranslationFromTarget
    (vec3 target)
    {
        mat4 mtx(1.0f);
        target.y += mFocusElevation;
        mtx = translate(mtx, target);
        mtx = rotate(mtx,mFocusPitch, vec3(1,0,0));
        mtx = rotate(mtx,mFocusYaw, vec3(0,1,0));
        mtx = translate(mtx,vec3(0,0,-mFocusRadius));
        mFocusTranslation = vec3(mtx[3]);
    }

    void Camera::deltaPitch(float pitch)
    {
        mPitch += pitch;
    }

    void Camera::deltaYaw(float yaw)
    {
        mYaw += yaw;
    }

    float Camera::getFocusedObjectTheta()
    {
        if (mFocusedSceneObject)
        {
            vec3 objTx = mFocusedSceneObject->getTransform().decomposeMatrix().translation;
            float x = mFocusTranslation.x - objTx.x;
            float z = mFocusTranslation.z - objTx.z;
            return atan2(x,z);
        }
        return 0.0f;
    }

    bool
    Camera::containedInFrustum
    (SceneObjectRuntime* sor)
    {
        return mFrustum.testIntersection(
            sor->getTransform().getMatrix(),
            sor->getBoundingBox()
        ) == Frustum::TEST_INSIDE;
    }

    bool
    Camera::containedInFrustum
    (const BoundingBox& bb)
    {
        const static mat4 tx(1.0f);
        return mFrustum.testIntersection(tx,bb) == Frustum::TEST_INSIDE;
    }

    bool
    Camera::exceedsFrustumPlaneAtTranslation
    (Frustum::Plane plane, SceneObjectRuntime* sor, const vec3& tx)
    {
        return mFrustum.testIntersectionWithPlane(plane,tx,sor->getBoundingBox()) != Frustum::TEST_INSIDE;
    }

    bool
    Camera::containedInFrustumAfterTransform
    (SceneObjectRuntime* sor, const mat4& tx)
    {
        return mFrustum.testIntersection(
            sor->getTransform().getMatrix() * tx,
            sor->getBoundingBox()
         ) != Frustum::TEST_OUTSIDE;

    }

    bool
    Camera::visibleInFrustum
    (SceneObjectRuntime* sor)
    {
        return mFrustum.testIntersection(
            sor->getTransform().getMatrix(),
            sor->getBoundingBox()
         ) != Frustum::TEST_OUTSIDE;
    }

    bool
    Camera::visibleInFrustum
    (const BoundingBox& bb)
    {
        const static mat4 tx(1.0f);
        return mFrustum.testIntersection(tx,bb) != Frustum::TEST_OUTSIDE;
    }


    mat4 Camera::getProjectionMatrix() const
    {
        return mProjectionMatrix;
    }

    void
    Camera::setProjectionMatrix
    (const mat4& projectionMatrix)
    {
        mProjectionMatrix = projectionMatrix;
    }

    void
    Camera::setFocusedSceneObejct
    (SceneObjectRuntime* rt)
    {
        mFocusedSceneObject = rt;
    }

    SceneObjectRuntime* Camera::getFocusedSceneObject() const
    {
        return mFocusedSceneObject;
    }

    float Camera::getFocusPitch() const
    {
        return mFocusPitch;
    }

    void Camera::setFocusPitch(float focusPitch)
    {
        mFocusPitch = focusPitch;
    }

    float Camera::getFocusYaw() const
    {
        return mFocusYaw;
    }

    void Camera::setFocusYaw(float focusYaw)
    {
        mFocusYaw = focusYaw;
    }

    float Camera::getFocusRadius() const
    {
        return mFocusRadius;
    }

    void Camera::setFocusRadius(float focusRadius)
    {
        mFocusRadius = focusRadius;
    }

    float Camera::getFocusElevation() const
    {
        return mFocusElevation;
    }

    void
    Camera::setFocusElevation
    (float focusElevation)
    {
        mFocusElevation = focusElevation;
    }

    float
    Camera::getMinimumDraw
    () const
    {
        return mMinimumDraw;
    }

    float
    Camera::getMaximumDraw
    () const
    {
        return mMaximumDraw;
    }

    void
    Camera::setMinimumDraw
    (float minimumDraw)
    {
        mMinimumDraw = minimumDraw;
    }

    void
    Camera::setMaximumDraw
    (float maximumDraw)
    {
        mMaximumDraw = maximumDraw;
    }

    float
    Camera::getMeshCullDistance
    ()
    const
    {
        return mMeshCullDistance;
    }

    void
    Camera::setMeshCullDistance
    (float meshCullDistance)
    {
        mMeshCullDistance = meshCullDistance;
    }

} // End of Dream
