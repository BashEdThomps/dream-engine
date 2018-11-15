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

namespace Dream
{
    Camera::Camera
    (vec3 translation, vec3 up, float yaw, float pitch)
        : DreamObject("Camera"),
        mTranslation(translation),
        mFront(vec3(0.0f, 0.0f, -1.0f)),
        mWorldUp(up),
        mFreeMode(true),
        mYaw(yaw),
        mPitch(pitch),
        mMovementSpeed(Constants::CAMERA_SPEED),
        mMouseSensitivity(Constants::CAMERA_SENSITIVTY),
        mZoom(Constants::CAMERA_ZOOM)
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
        if (mFreeMode)
        {
            return lookAt(mTranslation,mTranslation+mFront,mUp);
        }
        else
        {
            return lookAt(mTranslation,mLookAt,mUp);
        }
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

    vec3
    Camera::getRelativeTranslation
    (float relative)
    {
        return vec3
        (
            mTranslation.x + (mFront.x * relative),
            mTranslation.y + (mFront.y * relative),
            mTranslation.z + (mFront.z * relative)
        );
    }

    mat4
    Camera::getRelativeRotation
    (vec3 relativePosition)
    {
        return lookAt(relativePosition,getTranslation()-mFront,mUp);
    }

    void
    Camera::flyForward
    (float speed)
    {
        mTranslation.x += mFront.x * speed;
        mTranslation.y += mFront.y * speed;
        mTranslation.z += mFront.z * speed;
    }

    void
    Camera::flyZ
    (float speed)
    {
        mTranslation.x += mFront.x * speed;
        mTranslation.y += mFront.y * speed;
        mTranslation.z += mFront.z * speed;
    }

    void
    Camera::flyBackward
    (float speed)
    {
        mTranslation.x -= mFront.x * speed;
        mTranslation.y -= mFront.y * speed;
        mTranslation.z -= mFront.z * speed;
    }

    void
    Camera::flyLeft
    (float speed)
    {
        mTranslation.x -= mRight.x * speed;
        mTranslation.y -= mRight.y * speed;
        mTranslation.z -= mRight.z * speed;
    }

    void
    Camera::flyRight
    (float speed)
    {
        mTranslation.x += mRight.x * speed;
        mTranslation.y += mRight.y * speed;
        mTranslation.z += mRight.z * speed;
    }

    void
    Camera::flyX
    (float speed)
    {
        mTranslation.x += mRight.x * speed;
        mTranslation.y += mRight.y * speed;
        mTranslation.z += mRight.z * speed;
    }

    void
    Camera::flyUp
    (float speed)
    {
        mTranslation.y += speed;
    }

    void
    Camera::flyY
    (float speed)
    {
        mTranslation.y += speed;
    }

    void
    Camera::flyDown
    (float speed)
    {
        mTranslation.y -= speed;
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
        // mFreeMode == Don't enforce mLookAt
        if (mFreeMode)
        {
            mFront.x = static_cast<float>(cos(mYaw) * cos(mPitch));
            mFront.y = static_cast<float>(sin(mPitch));
            mFront.z = static_cast<float>(sin(mYaw) * cos(mPitch));
            mFront = normalize(mFront);
            mRight = normalize(cross(mFront, mWorldUp));
            mUp    = mWorldUp;
        }
        else
        {
            mFront = normalize(mLookAt);
            mRight = normalize(cross(mFront, mWorldUp));
            mUp    = mWorldUp;
        }
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

    void Camera::setLookAt(float x, float y, float z)
    {
        mLookAt.x = x;
        mLookAt.y = y;
        mLookAt.z = z;
    }

    void
    Camera::setLookAt
    (vec3 transform)
    {
        mLookAt = transform;
    }

    glm::vec3
    Camera::getLookAt
    ()
    {
       return mLookAt;
    }

    void
    Camera::setFreeMode
    (bool freemode)
    {
       mFreeMode = freemode;
    }

    void
    Camera::orbit
    (vec3 target, float elevation, float radius, float pitch, float yaw)
    {
        mat4 mtx(1.0f);
        target.y += elevation;
        mtx = translate(mtx, target);
        mtx = rotate(mtx,pitch, vec3(1,0,0));
        mtx = rotate(mtx,yaw, vec3(0,1,0));
        mtx = translate(mtx,vec3(0,0,-radius));
        mTranslation = vec3(mtx[3]);
    }

    void Camera::deltaPitch(float pitch)
    {
        mPitch += pitch;
    }

    void Camera::deltaYaw(float yaw)
    {
        mYaw += yaw;
    }

} // End of Dream
