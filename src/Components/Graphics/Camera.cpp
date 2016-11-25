#include "Camera.h"

namespace Dream {
  namespace Components {
    namespace Graphics {
      // Constructor with vectors
      Camera::Camera(std::vector<float> translation,
                     std::vector<float> up,
                     float yaw,
                     float pitch) {
        mFront = { 0.0f, 0.0f, -1.0f };
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
        mTranslation      = { posX, posY, posZ };
        mWorldUp          = { upX,  upY,  upZ  };
        mYaw              = yaw;
        mPitch            = pitch;
        updateCameraVectors();
      }

      std::vector<std::vector<float>> Camera::getViewMatrix() {
        std::vector<float> translationFrontSum = {
          mTranslation[0] + mFront[0],
          mTranslation[1] + mFront[1],
          mTranslation[2] + mFront[2]
        };
        return lookAt(mTranslation, translationFrontSum, mUp);
      }

      void Camera::processKeyboard(const int direction, const float deltaTime) {
        float velocity = mMovementSpeed * deltaTime;

        if (direction == CAMERA_MOVEMENT_FORWARD) {
          mTranslation[0] += mFront[0] * velocity;
          mTranslation[1] += mFront[1] * velocity;
          mTranslation[2] += mFront[2] * velocity;
        }

        if (direction == CAMERA_MOVEMENT_BACKWARD) {
          mTranslation[0] -= mFront[0] * velocity;
          mTranslation[1] -= mFront[1] * velocity;
          mTranslation[2] -= mFront[2] * velocity;
        }

        if (direction == CAMERA_MOVEMENT_LEFT) {
          mTranslation[0] -= mRight[0] * velocity;
          mTranslation[1] -= mRight[1] * velocity;
          mTranslation[2] -= mRight[2] * velocity;
        }

        if (direction == CAMERA_MOVEMENT_RIGHT) {
          mTranslation[0] += mRight[0] * velocity;
          mTranslation[1] += mRight[1] * velocity;
          mTranslation[2] += mRight[2] * velocity;
        }
      }

      std::vector<float> Camera::getTranslation() {
        return mTranslation;
      }

      std::vector<float> Camera::getRotation() {
        std::vector<float> retval(3);
        retval[0] = mPitch;
        retval[1] = mYaw;
        retval[2] = 0.0f;
        return retval;
      }

      void Camera::processMouseMovement(const float xoffset, const float yoffset,
                                        const bool constrainPitch = true) {
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

      void Camera::processMouseScroll(const float yoffset) {
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

        //std::cout << "Camera: Zoom is " << mZoom << std::endl;
      }

      void Camera::updateCameraVectors() {
        mFront[0] = static_cast<float>(cos(radians(mYaw)) * cos(radians(mPitch)));
        mFront[1] =	static_cast<float>(sin(radians(mPitch)));
        mFront[2] =	static_cast<float>(sin(radians(mYaw)) * cos(radians(mPitch)));
        mFront = normalize(mFront);
        mRight = normalize(cross(mFront, mWorldUp));
        mUp    = normalize(cross(mRight, mFront));
      }

      void Camera::setRotation(std::vector<float> rotation) {}

      void Camera::setTranslation(std::vector<float> translation) {
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

      std::vector<float> Camera::cross(std::vector<float> x, std::vector<float> y) {
        std::vector<float> retval(3);
        retval[0] = x[1] * y[2] - y[1] * x[2];
        retval[1] = x[2] * y[0] - y[2] * x[0];
        retval[2] = x[0] * y[1] - y[0] * x[1];
        return retval;
      }

      float Camera::radians(float degrees) {
        return degrees * 0.01745329251994329576923690768489f;
      }

      std::vector<std::vector<float>> Camera::lookAt (std::vector<float> eye, std::vector<float> center, std::vector<float> up) {
        std::vector<std::vector<float>> retval(4);

        glm::vec3 glmEye;
        glmEye.x = eye[0];
        glmEye.y = eye[1];
        glmEye.z = eye[2];

        glm::vec3 glmCenter;
        glmCenter.x = center[0];
        glmCenter.y = center[1];
        glmCenter.z = center[2];

        glm::vec3 glmUp;
        glmUp.x = up[0];
        glmUp.y = up[1];
        glmUp.z = up[2];

        glm::mat4 lookAtMatrix = glm::lookAt(glmEye, glmCenter, glmUp);
        retval[0] = {lookAtMatrix[0][0],lookAtMatrix[0][1],lookAtMatrix[0][2],lookAtMatrix[0][3]};
        retval[1] = {lookAtMatrix[1][0],lookAtMatrix[1][1],lookAtMatrix[1][2],lookAtMatrix[1][3]};
        retval[2] = {lookAtMatrix[2][0],lookAtMatrix[2][1],lookAtMatrix[2][2],lookAtMatrix[2][3]};
        retval[3] = {lookAtMatrix[3][0],lookAtMatrix[3][1],lookAtMatrix[3][2],lookAtMatrix[3][3]};

        return retval;
      }

      float Camera::dot(std::vector<float> a, std::vector<float> b) {
        return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
      }

      // Returns the length of the vector
      float Camera::vectorLength(std::vector<float> vector){
        return sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
      }

      // Normalizes the vector
      std::vector<float> Camera::normalize(std::vector<float> vector){
        std::vector<float> retval(3);
        float length = vectorLength(vector);
        if(length != 0){
          retval[0] = vector[0]/length;
          retval[1] = vector[1]/length;
          retval[2] = vector[2]/length;
        }
        return retval;
      }
    } // End of Graphics
  } // End of Components
} // End of Dream
