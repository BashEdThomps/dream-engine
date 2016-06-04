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
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Dream   {
namespace Plugins {
namespace Video   {
namespace OpenGL  {
	
	
	// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
	class Camera {
	public:
		// Camera Attributes
		glm::vec3 mPosition;
		glm::vec3 mFront;
		glm::vec3 mUp;
		glm::vec3 mRight;
		glm::vec3 mWorldUp;
		// Eular Angles
		GLfloat mYaw;
		GLfloat mPitch;
		// Camera options
		GLfloat mMovementSpeed;
		GLfloat mMouseSensitivity;
		GLfloat mZoom;
		
		// Constructor with vectors
		Camera(
		  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		  glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f),
		  GLfloat   yaw      = YAW,
		  GLfloat   pitch    = PITCH
		);
		
		// Constructor with scalar values
		Camera(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
		
		// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
		glm::mat4 getViewMatrix();
		
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
		
	}; // End of Camera
	
} // End of OpenGL
} // End of Video
} // End of Plugins
} // End of Dream
#endif  // DSGCAMERA_H
