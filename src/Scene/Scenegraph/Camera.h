#ifndef DSGCAMERA_H
#define DSGCAMERA_H

#include <stdlib.h>

#define DSG_CAM_X 0
#define DSG_CAM_Y 1
#define DSG_CAM_Z 2

namespace Dream {
    namespace Scene {
        namespace Scenegraph {
            class Camera {
            public:
                float mLookAt      [3];
                float mUp          [3];
                float mTranslation [3];
            public:
                Camera();
                ~Camera();

                void setLookAt      (float, float, float);
                void setUp          (float, float, float);
                void setTranslation (float, float, float);
            };
        }
    }
}
#endif  // DSGCAMERA_H
