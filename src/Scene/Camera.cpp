#include "Camera.h"

namespace Dream {
namespace Scene {
	
	
		// Constructor with vectors
	Camera::Camera(std::vector<float> translation, std::vector<float> up, float yaw, float pitch) {
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
	Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) {
		mFront = {0.0f, 0.0f, -1.0f};
		mMovementSpeed = SPEED;
		mMouseSensitivity = SENSITIVTY;
		mZoom = ZOOM;
		mTranslation = { posX, posY, posZ };
		mWorldUp  = { upX,  upY,  upZ  };
		mYaw = yaw;
		mPitch = pitch;
		updateCameraVectors();
	}
		
	std::vector<std::vector<double>> Camera::getViewMatrix() {
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
		return {mPitch,mYaw,0.0f};
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
		std::vector<float> front(3);
		front[0] = cos(radians(mYaw)) * cos(radians(mPitch));
		front[1] = sin(radians(mPitch));
		front[2] = sin(radians(mYaw)) * cos(radians(mPitch));
		mFront = normalize(front);
		// Also re-calculate the Right and Up vector
		// Normalize the vectors, because their length gets closer to 0 the more you look up or
		// down which results in slower movement.
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
		return {
			x[1] * y[2] - y[1] * x[2],
			x[2] * y[0] - y[2] * x[0],
			x[0] * y[1] - y[0] * x[1]
		};
	}
	
	std::vector<float> Camera::normalize(std::vector<float> x) {
		//return x < genType(0) ? genType(-1) : genType(1);
		return {
			x[0] < 0.0f ? -1.0f : 1.0f,
			x[1] < 0.0f ? -1.0f : 1.0f,
			x[2] < 0.0f ? -1.0f : 1.0f
		};
	}
	
	float Camera::radians(float degrees) {
		return degrees * 0.01745329251994329576923690768489f;
	}
	
	std::vector<std::vector<double>> Camera::lookAt (std::vector<float> eye, std::vector<float> center, std::vector<float> up) {
		std::vector<float> f =
			normalize({
  			center[0] - eye[0],
  			center[1] - eye[1],
  			center[2] - eye[2]
		});
		
		std::vector<float> s = normalize(cross(f, up));
		std::vector<float> u = cross(s, f);
		
		std::vector<std::vector<double>> Result = {
			{s[0],u[0],-f[0]},
			{s[1],u[1],-f[1]},
			{s[2],u[2],-f[2]},
			{
				(-std::inner_product(begin(s), end(s), begin(eye), 0.0)), // -dot(s, eye);
				(-std::inner_product(begin(u), end(u), begin(eye), 0.0)), // -dot(u, eye);
				(std::inner_product(begin(f), end(f), begin(eye), 0.0)) //dot(f, eye);
			}
		};
		return Result;
	}
	
	
} // End of Scene
} // End of Dream
