#include "Camera.h"

namespace Dream {
      // Constructor with vectors
      Camera::Camera(glm::vec3 translation, glm::vec3 up, float yaw, float pitch) {
        mFront = glm::vec3(0.0f, 0.0f, -1.0f);
        mMovementSpeed = SPEED;
        mMouseSensitivity = SENSITIVTY;
        mZoom = ZOOM;
        mTranslation = translation;
        mWorldUp = up;
        mYaw = yaw;
        mPitch = pitch;
        updateCameraVectors();
      }

      // Constructor with scalar values
      Camera::Camera(float posX, float posY, float posZ,
                     float upX, float upY, float upZ,
                     float yaw, float pitch) {
        mFront            = {0.0f, 0.0f, -1.0f};
        mMovementSpeed    = SPEED;
        mMouseSensitivity = SENSITIVTY;
        mZoom             = ZOOM;
        mTranslation      = glm::vec3(posX, posY, posZ);
        mWorldUp          = glm::vec3(upX,  upY,  upZ);
        mYaw              = yaw;
        mPitch            = pitch;
        updateCameraVectors();
      }

      Camera::~Camera() {
        if (DEBUG) {
            cout << "Camera: Destroying Object" << endl;
        }
        return;
      }

      glm::mat4 Camera::getViewMatrix() {
          return glm::lookAt(mTranslation,mTranslation+mFront,mUp);
      }

      void Camera::processKeyboard(int direction, float deltaTime) {
        float velocity = mMovementSpeed * deltaTime;

        if (direction == CAMERA_MOVEMENT_FORWARD) {
          mTranslation.x += mFront.x * velocity;
          mTranslation.y += mFront.y * velocity;
          mTranslation.z += mFront.z * velocity;
        }

        if (direction == CAMERA_MOVEMENT_BACKWARD) {
          mTranslation.x -= mFront.x * velocity;
          mTranslation.y -= mFront.y * velocity;
          mTranslation.z -= mFront.z * velocity;
        }

        if (direction == CAMERA_MOVEMENT_LEFT) {
          mTranslation.x -= mRight.x * velocity;
          mTranslation.y -= mRight.y * velocity;
          mTranslation.z -= mRight.z * velocity;
        }

        if (direction == CAMERA_MOVEMENT_RIGHT) {
          mTranslation.x += mRight.x * velocity;
          mTranslation.y += mRight.y * velocity;
          mTranslation.z += mRight.z * velocity;
        }
      }

      glm::vec3 Camera::getTranslation() {
        return mTranslation;
      }

      glm::vec3 Camera::getRotation() {
        return glm::vec3 (mPitch,mYaw,0.0f);
      }

      void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
        mYaw   += xoffset * mMouseSensitivity;
        mPitch -= yoffset * mMouseSensitivity;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
          if (mPitch > PITCH_MAX) {
            mPitch = PITCH_MAX;
          }
          if (mPitch < -PITCH_MAX) {
            mPitch = -PITCH_MAX;
          }
        }

        // Update Front, Right and Up Vectors using the updated Eular angles
        updateCameraVectors();
      }

      void Camera::processMouseScroll(float yoffset) {
        if (yoffset == 0.0f) {
          return;
        }

        if (mZoom >= ZOOM_MIN && mZoom <= ZOOM_MAX) {
          mZoom -= yoffset;
        }

        if (mZoom <= ZOOM_MIN) {
          mZoom = ZOOM_MIN;
        }

        if (mZoom >= ZOOM_MAX) {
          mZoom = ZOOM_MAX;
        }
        if (DEBUG) {
            cout << "Camera: Zoom is " << mZoom << endl;
        }
      }

      void Camera::updateCameraVectors() {
        mFront.x = static_cast<float>(cos(radians(mYaw)) * cos(radians(mPitch)));
        mFront.y = static_cast<float>(sin(radians(mPitch)));
        mFront.z = static_cast<float>(sin(radians(mYaw)) * cos(radians(mPitch)));
        mFront = glm::normalize(mFront);
        mRight = glm::normalize(glm::cross(mFront, mWorldUp));
        mUp    = glm::normalize(glm::cross(mRight, mFront));
      }

      void Camera::setRotation(glm::vec3 rotation) {
        mPitch = rotation.x;
        mYaw = rotation.y;
        updateCameraVectors();
      }

      void Camera::setTranslation(glm::vec3 translation) {
        mTranslation = translation;
      }

      void Camera::setMovementSpeed(float movementSpeed) {
        mMovementSpeed = movementSpeed;
      }

      float Camera::getMovementSpeed() {
        return mMovementSpeed;
      }

      void Camera::setMouseSensitivity(float mouseSensitivity) {
        mMouseSensitivity = mouseSensitivity;
      }

      float Camera::getMouseSensitivity() {
        return mMouseSensitivity;
      }

      float Camera::getZoom() {
        return mZoom;
      }

      float Camera::radians(float degrees) {
        return degrees * 0.01745329251994329576923690768489f;
      }

} // End of Dream
