#ifndef CAMERA_H
#define CAMERA_H

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

#define YAW       0.0f
#define PITCH      0.0f
#define SPEED      10.0f
#define SENSITIVTY 0.125f
#define ZOOM       45.0f

#include "../../Constants.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

using namespace std;

namespace Dream
{
    class Camera
    {
    private:
        // Camera Attributes
        glm::vec3 mTranslation;
        glm::vec3 mFront;
        glm::vec3 mUp;
        glm::vec3 mRight;
        glm::vec3 mWorldUp;
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
                glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                float   yaw      = YAW,
                float   pitch    = PITCH
                );

        // Constructor with scalar values
        Camera(float, float, float, float, float, float, float, float);
        ~Camera();
        glm::mat4 getViewMatrix();
        void processKeyboard(int, float);
        void processMouseMovement(float, float, bool);
        void processMouseScroll(float);
        void updateCameraVectors();
        void setTranslation(glm::vec3);
        glm::vec3 getTranslation();
        void setRotation(glm::vec3);
        glm::vec3 getRotation();
        void  setMovementSpeed(float);
        float getMovementSpeed();
        void  setMouseSensitivity(float);
        float getMouseSensitivity();
        float getZoom();
    private:
        float radians(float);

    }; // End of Camera

} // End of Dream

#endif  // CAMERA_H
