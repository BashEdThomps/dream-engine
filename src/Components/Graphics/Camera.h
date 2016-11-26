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

#define YAW       -90.0f
#define PITCH      0.0f
#define SPEED      10.0f
#define SENSITIVTY 0.125f
#define ZOOM       45.0f

#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

namespace Dream {

  using namespace std;

  class Camera {
  private:
    // Camera Attributes
    vector<float> mTranslation;
    vector<float> mFront;
    vector<float> mUp;
    vector<float> mRight;
    vector<float> mWorldUp;
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
        vector<float> position = { 0.0f, 0.0f, 0.0f},
        vector<float> up       = { 0.0f, 1.0f, 0.0f},
        float   yaw      = YAW,
        float   pitch    = PITCH
        );

    // Constructor with scalar values
    Camera(float, float, float, float, float, float, float, float);
    vector<vector<float>> getViewMatrix();
    void processKeyboard(const int, const float);
    void processMouseMovement(const float, const float, const bool);
    void processMouseScroll(const float);
    void updateCameraVectors();
    void setTranslation(vector<float>);
    vector<float> getTranslation();
    void setRotation(vector<float>);
    vector<float> getRotation();
    void  setMovementSpeed(float);
    float getMovementSpeed();
    void  setMouseSensitivity(float);
    float getMouseSensitivity();
    float getZoom();
  private:
    float radians(float);
    vector<float> cross(vector<float>, vector<float>);
    vector<float> normalize(vector<float>);
    vector<vector<float>> lookAt(vector<float>,vector<float>,vector<float>);
    float dot(vector<float>,vector<float>);
    float vectorLength(vector<float>);

  }; // End of Camera

} // End of Dream

#endif  // CAMERA_H
