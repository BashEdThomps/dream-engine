#ifndef DSGCAMERA_H
#define DSGCAMERA_H

#define DSG_CAM_X 0
#define DSG_CAM_Y 1
#define DSG_CAM_Z 2
#define PITCH_MAX 75.0f

#define	CAMERA_MOVEMENT_FORWARD  0
#define	CAMERA_MOVEMENT_BACKWARD 1
#define	CAMERA_MOVEMENT_LEFT     2
#define	CAMERA_MOVEMENT_RIGHT    3

#define ZOOM_MIN 1.0f
#define ZOOM_MAX 45.0f

#define YAW       -90.0f
#define PITCH      0.0f
#define SPEED      10.0f
#define SENSITIVTY 0.125f
#define ZOOM       45.0f

#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>

namespace Dream {
namespace Scene {
	
	class Camera {
	private:
		// Camera Attributes
		std::vector<float> mTranslation;
		std::vector<float> mFront;
		std::vector<float> mUp;
		std::vector<float> mRight;
		std::vector<float> mWorldUp;
		// Eular Angles
		float mYaw;
		float mPitch;
		// Camera options
		float mMovementSpeed;
		float mMouseSensitivity;
		float mZoom;
	public:
		// Constructor with vectors
		Camera(
			std::vector<float> position = { 0.0f, 0.0f, 0.0f},
			std::vector<float> up       = { 0.0f, 1.0f, 0.0f},
		  float   yaw      = YAW,
		  float   pitch    = PITCH
		);
		
		// Constructor with scalar values
		Camera(float, float, float, float, float, float, float, float);
		
		// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
		std::vector<std::vector<double>> getViewMatrix();
		
		// Processes input received from any keyboard-like input system.
		// Accepts input parameter in the form of camera defines (to abstract it from windowing systems)
		void processKeyboard(const int, const float);
		
		// Processes input received from a mouse input system.
		// Expects the offset value in both the x and y direction.
		void processMouseMovement(const float, const float, const bool);
		
		// Processes input received from a mouse scroll-wheel event.
		// Only requires input on the vertical wheel-axis
		void processMouseScroll(const float);
		
		// Calculates the front vector from the Camera's (updated) Eular Angles
		void updateCameraVectors();
		
		void setTranslation(std::vector<float>);
		std::vector<float> getTranslation();
		
		void setRotation(std::vector<float>);
		std::vector<float> getRotation();
		
		void  setMovementSpeed(float);
		float getMovementSpeed();
		
		void  setMouseSensitivity(float);
		float getMouseSensitivity();
		
		float getZoom();
	private:
		std::vector<float> cross(std::vector<float>, std::vector<float>);
		std::vector<float> normalize(std::vector<float>);
		float radians(float);
		std::vector<std::vector<double>> lookAt(std::vector<float>,std::vector<float>,std::vector<float>);
		
		
	}; // End of Camera
	
} // End of Scene
} // End of Dream

#endif  // DSGCAMERA_H
