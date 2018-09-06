#include "Camera.h"

namespace Dream
{
    // Constructor with vectors
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

    Camera::Camera
    (vec3 translation, vec3 up, float yaw, float pitch)
        : DreamObject("Camera"),
          mFreeMode(true)
    {
        mFront = vec3(0.0f, 0.0f, -1.0f);
        mMovementSpeed = Constants::CAMERA_SPEED;
        mMouseSensitivity = Constants::CAMERA_SENSITIVTY;
        mZoom = Constants::CAMERA_ZOOM;
        mTranslation = translation;
        mWorldUp = up;
        mYaw = yaw;
        mPitch = pitch;
        updateCameraVectors();
    }

    // Constructor with scalar values
    Camera::Camera
    (
            float posX, float posY, float posZ,
            float upX, float upY, float upZ,
            float yaw, float pitch
            )
        :DreamObject ("Camera"),
        mFreeMode(true)
    {
        mFront            = {0.0f, 0.0f, -1.0f};
        mMovementSpeed    = Constants::CAMERA_SPEED;
        mMouseSensitivity = Constants::CAMERA_SENSITIVTY;
        mZoom             = Constants::CAMERA_ZOOM;
        mTranslation      = vec3(posX, posY, posZ);
        mWorldUp          = vec3(upX,  upY,  upZ);
        mYaw              = yaw;
        mPitch            = pitch;
        updateCameraVectors();
    }

    Camera::~Camera
    ()
    {
        auto log = getLog();
        log->info("Destroying Object");
        return;
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
    Camera::processKeyboard
    (unsigned int direction, float deltaTime)
    {
        float velocity = mMovementSpeed * deltaTime;

        if (direction == Constants::CAMERA_MOVEMENT_FORWARD)
        {
            mTranslation.x += mFront.x * velocity;
            mTranslation.y += mFront.y * velocity;
            mTranslation.z += mFront.z * velocity;
        }

        if (direction == Constants::CAMERA_MOVEMENT_BACKWARD)
        {
            mTranslation.x -= mFront.x * velocity;
            mTranslation.y -= mFront.y * velocity;
            mTranslation.z -= mFront.z * velocity;
        }

        if (direction == Constants::CAMERA_MOVEMENT_LEFT)
        {
            mTranslation.x -= mRight.x * velocity;
            mTranslation.y -= mRight.y * velocity;
            mTranslation.z -= mRight.z * velocity;
        }

        if (direction == Constants::CAMERA_MOVEMENT_RIGHT)
        {
            mTranslation.x += mRight.x * velocity;
            mTranslation.y += mRight.y * velocity;
            mTranslation.z += mRight.z * velocity;
        }

        if (direction == Constants::CAMERA_MOVEMENT_DOWN)
        {
            mTranslation.y -= velocity;
        }

        if (direction == Constants::CAMERA_MOVEMENT_UP)
        {
            mTranslation.y += velocity;
        }
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
    Camera::pan
    (float xoffset, float yoffset, bool constrainPitch)
    {
        mYaw   += xoffset;
        mPitch -= yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (mPitch > Constants::CAMERA_PITCH_MAX)
            {
                mPitch = Constants::CAMERA_PITCH_MAX;
            }

            if (mPitch < -Constants::CAMERA_PITCH_MAX)
            {
                mPitch = -Constants::CAMERA_PITCH_MAX;
            }
        }

        mYaw = fmodf(mYaw,static_cast<float>(M_PI)*2);
        mPitch = fmodf(mPitch,static_cast<float>(M_PI)*2);

        // Update Front, Right and Up Vectors using the updated Eular angles
        updateCameraVectors();
    }

    void
    Camera::processMouseMovement
    (float xoffset, float yoffset, bool constrainPitch)
    {
        mYaw   += xoffset * mMouseSensitivity;
        mPitch -= yoffset * mMouseSensitivity;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (mPitch > Constants::CAMERA_PITCH_MAX)
            {
                mPitch = Constants::CAMERA_PITCH_MAX;
            }

            if (mPitch < -Constants::CAMERA_PITCH_MAX)
            {
                mPitch = -Constants::CAMERA_PITCH_MAX;
            }
        }

        mYaw = fmodf(mYaw,static_cast<float>(M_PI)*2);
        mPitch = fmodf(mPitch,static_cast<float>(M_PI)*2);

        // Update Front, Right and Up Vectors using the updated Eular angles
        updateCameraVectors();
    }

    void
    Camera::processMouseScroll
    (float yoffset)
    {
        auto log = getLog();
        if (yoffset == 0.0f)
        {
            return;
        }

        if (mZoom >= Constants::CAMERA_ZOOM_MIN && mZoom <= Constants::CAMERA_ZOOM_MAX)
        {
            mZoom -= yoffset;
        }

        if (mZoom <= Constants::CAMERA_ZOOM_MIN)
        {
            mZoom = Constants::CAMERA_ZOOM_MIN;
        }

        if (mZoom >= Constants::CAMERA_ZOOM_MAX)
        {
            mZoom = Constants::CAMERA_ZOOM_MAX;
        }
        log->info("Zoom is {}" ,mZoom );
    }

    void
    Camera::updateCameraVectors
    ()
    {
        if (!mFreeMode)
        {
            mFront = normalize(mLookAt);
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
            mUp    = mWorldUp;//normalize(cross(mRight, mFront));
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

    void
    Camera::setMouseSensitivity
    (float mouseSensitivity)
    {
        mMouseSensitivity = mouseSensitivity;
    }

    float
    Camera::getMouseSensitivity
    ()
    {
        return mMouseSensitivity;
    }

    float
    Camera::getZoom
    ()
    {
        return mZoom;
    }

    void Camera::setLookAt(float x, float y, float z)
    {
        mLookAt.x = x;
        mLookAt.y = y;
        mLookAt.z = z;
    }

    void Camera::setLookAt(vec3 transform)
    {
        mLookAt = transform;
    }

    glm::vec3 Camera::getLookAt()
    {
       return mLookAt;
    }

    void Camera::setFreeMode(bool freemode)
    {
       mFreeMode = freemode;
    }

    void
    Camera::orbit
    (vec3 target, float elevation, float radius, float pitch, float yaw)
    {
        mat4 mtx;
        target.y += elevation;
        mtx = translate(mtx, target);
        mtx = rotate(mtx,pitch, vec3(1,0,0));
        mtx = rotate(mtx,yaw, vec3(0,1,0));
        mtx = translate(mtx,vec3(0,0,-radius));
        mTranslation = vec3(mtx[3]);
    }

} // End of Dream
