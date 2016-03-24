#ifndef DSGCAMERA_H
#define DSGCAMERA_H

#include <stdlib.h>

#define DSG_CAM_X 0
#define DSG_CAM_Y 1
#define DSG_CAM_Z 2

namespace DreamSceneGraph {
    class Camera {
    public:
        float lookAt      [3];
        float up          [3];
        float translation [3];
    public:
        Camera();
        ~Camera();

        void setLookAt      (float, float, float);
        void setUp          (float, float, float);
        void setTranslation (float, float, float);
    };
}

#endif  // DSGCAMERA_H
