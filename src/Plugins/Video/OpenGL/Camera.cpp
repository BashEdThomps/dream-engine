#include "Camera.h"

namespace Dream   {
namespace Plugins {
namespace Video   {
namespace OpenGL  {
	
		// Constructor with vectors
	Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) {
		mFront = glm::vec3(0.0f, 0.0f, -1.0f);
		mMovementSpeed = SPEED;
		mMouseSensitivity = SENSITIVTY;
		mZoom = ZOOM;
		mPosition = position;
		mWorldUp = up;
		mYaw = yaw;
		mPitch = pitch;
		updateCameraVectors();
	}
	
	// Constructor with scalar values
	Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
								 GLfloat upX, GLfloat upY, GLfloat upZ,
								 GLfloat yaw, GLfloat pitch) {
		mFront = glm::vec3(0.0f, 0.0f, -1.0f);
		mMovementSpeed = SPEED;
		mMouseSensitivity = SENSITIVTY;
		mZoom = ZOOM;
		mPosition = glm::vec3(posX, posY, posZ);
		mWorldUp = glm::vec3(upX, upY, upZ);
		mYaw = yaw;
		mPitch = pitch;
		updateCameraVectors();
	}
		
	glm::mat4 Camera::getViewMatrix() {
		return glm::lookAt(mPosition, mPosition + mFront, mUp);
	}
	
	void Camera::processKeyboard(const int direction, const float deltaTime) {
		GLfloat velocity = mMovementSpeed * deltaTime;
		
		if (direction == CAMERA_MOVEMENT_FORWARD) {
			mPosition += mFront * velocity;
		}
		
		if (direction == CAMERA_MOVEMENT_BACKWARD) {
			mPosition -= mFront * velocity;
		}
		
		if (direction == CAMERA_MOVEMENT_LEFT) {
			mPosition -= mRight * velocity;
		}
		
		if (direction == CAMERA_MOVEMENT_RIGHT) {
			mPosition += mRight * velocity;
		}
	}
	
	void Camera::processMouseMovement(const float xoffset, const float yoffset, const bool constrainPitch = true) {
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
		
    #ifdef VERBOSE
    	std::cout << "Camera: Zoom is " << mZoom << std::endl;
    #endif
	}
		
	void Camera::updateCameraVectors() {
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		front.y = sin(glm::radians(mPitch));
		front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		mFront = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		// Normalize the vectors, because their length gets closer to 0 the more you look up or
		// down which results in slower movement.
		mRight = glm::normalize(glm::cross(mFront, mWorldUp));
		mUp    = glm::normalize(glm::cross(mRight, mFront));
	}
	
	void Camera::setRotation(std::vector<float> rotation) {}
	
	void Camera::setTranslation(std::vector<float> translation) {
		mPosition = glm::vec3 (translation[0],translation[1],translation[2]);
	}
	
} // End of OpenGL
} // End of Video
} // End of Plugins
} // End of Dream
