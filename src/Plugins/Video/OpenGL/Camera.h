#ifndef DSGCAMERA_H
#define DSGCAMERA_H

#define DSG_CAM_X 0
#define DSG_CAM_Y 1
#define DSG_CAM_Z 2

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Dream   {
namespace Plugins {
namespace Video   {
namespace OpenGL  {
				
	class Camera {
  public:
		float mLookAt      [3];
    float mUp          [3];
    float mTranslation [3];
  public:
    Camera();
    ~Camera();
    void   setLookAt (float, float, float);
    float* getLookAt ();
    void   setUp (float, float, float);
    float* getUp ();
    void   setTranslation (float, float, float);
    float* getTranslation ();
		glm::mat4 getViewMatrix();
		glm::mat4 calculateModelViewProjection();
	};
	
}
}
}
}
#endif  // DSGCAMERA_H
