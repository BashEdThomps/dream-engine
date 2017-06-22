#include "Camera.h"

namespace Dream
{
    // Constructor with vectors
    Camera::Camera
    (vec3 translation, vec3 up, float yaw, float pitch)
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
        if (Constants::DEBUG)
        {
            cout << "Camera: Destroying Object" << endl;
        }
        return;
    }

    mat4
    Camera::getViewMatrix
    ()
    {
        return lookAt(mTranslation,mTranslation+mFront,mUp);
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
    }

    vec3
    Camera::getTranslation
    ()
    {
        return mTranslation;
    }

    vec3
    Camera::getRotation
    ()
    {
        return vec3 (mPitch,mYaw,0.0f);
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

        // Update Front, Right and Up Vectors using the updated Eular angles
        updateCameraVectors();
    }

    void
    Camera::processMouseScroll
    (float yoffset)
    {
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

        if (Constants::DEBUG)
        {
            cout << "Camera: Zoom is " << mZoom << endl;
        }
    }

    void
    Camera::updateCameraVectors
    ()
    {
        float adjustedYaw = mYaw + Constants::CAMERA_YAW_OFFSET;
        mFront.x = static_cast<float>(cos(radians(adjustedYaw)) * cos(radians(mPitch)));
        mFront.y = static_cast<float>(sin(radians(mPitch)));
        mFront.z = static_cast<float>(sin(radians(adjustedYaw)) * cos(radians(mPitch)));
        mFront = normalize(mFront);
        mRight = normalize(cross(mFront, mWorldUp));
        mUp    = normalize(cross(mRight, mFront));
    }

    void
    Camera::setRotation
    (vec3 rotation)
    {
        mPitch = rotation.x;
        mYaw = rotation.y;
        updateCameraVectors();
    }

    void
    Camera::setTranslation
    (vec3 translation)
    {
        mTranslation = translation;
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

    void
    Camera::setTransform
    (Transform3D transform)
    {
       setTranslation(transform.getTranslation());
       setRotation(transform.getRotation());
    }

    float
    Camera::radians
    (float degrees)
    {
        return degrees * 0.01745329251994329576923690768489f;
    }

} // End of Dream
