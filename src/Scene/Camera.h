#ifndef DSGCAMERA_H
#define DSGCAMERA_H

#include <stdlib.h>

#define DSG_CAM_X 0
#define DSG_CAM_Y 1
#define DSG_CAM_Z 2

namespace Dream {
    namespace Scene {
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
        };
    }
}
#endif  // DSGCAMERA_H
