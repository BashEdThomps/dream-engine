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
    ()
    {
        mTranslation.x += mFront.x * mMovementSpeed;
        mTranslation.y += mFront.y * mMovementSpeed;
        mTranslation.z += mFront.z * mMovementSpeed;
    }

    void
    Camera::flyZ
    ()
    {
        mTranslation.x += mFront.x * mMovementSpeed;
        mTranslation.y += mFront.y * mMovementSpeed;
        mTranslation.z += mFront.z * mMovementSpeed;
    }

    void
    Camera::flyBackward
    ()
    {
        mTranslation.x -= mFront.x * mMovementSpeed;
        mTranslation.y -= mFront.y * mMovementSpeed;
        mTranslation.z -= mFront.z * mMovementSpeed;
    }

    void
    Camera::flyLeft
    ()
    {
        mTranslation.x -= mRight.x * mMovementSpeed;
        mTranslation.y -= mRight.y * mMovementSpeed;
        mTranslation.z -= mRight.z * mMovementSpeed;
    }

    void
    Camera::flyRight
    ()
    {
        mTranslation.x += mRight.x * mMovementSpeed;
        mTranslation.y += mRight.y * mMovementSpeed;
        mTranslation.z += mRight.z * mMovementSpeed;
    }

    void
    Camera::flyX
    ()
    {
        mTranslation.x += mRight.x * mMovementSpeed;
        mTranslation.y += mRight.y * mMovementSpeed;
        mTranslation.z += mRight.z * mMovementSpeed;
    }

    void
    Camera::flyUp
    ()
    {
        mTranslation.y += mMovementSpeed;
    }

    void
    Camera::flyY
    ()
    {
        mTranslation.y += mMovementSpeed;
    }

    void
    Camera::flyDown
    ()
    {
        mTranslation.y -= mMovementSpeed;
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

    /*
void Frustum(float left, float right, float bottom, float top, float znear, float zfar, float *m16)
{
    float temp, temp2, temp3, temp4;
    temp = 2.0f * znear;
    temp2 = right - left;
    temp3 = top - bottom;
    temp4 = zfar - znear;
    m16[0] = temp / temp2;
    m16[1] = 0.0;
    m16[2] = 0.0;
    m16[3] = 0.0;
    m16[4] = 0.0;
    m16[5] = temp / temp3;
    m16[6] = 0.0;
    m16[7] = 0.0;
    m16[8] = (right + left) / temp2;
    m16[9] = (top + bottom) / temp3;
    m16[10] = (-zfar - znear) / temp4;
    m16[11] = -1.0f;
    m16[12] = 0.0;
    m16[13] = 0.0;
    m16[14] = (-temp * zfar) / temp4;
    m16[15] = 0.0;
}
*/

} // End of Dream
