#include "Camera.h"

namespace Dream {
    namespace Scene {
        namespace Scenegraph {
            Camera::Camera() {
                mLookAt[0] = 0.0f;
                mLookAt[1] = 0.0f;
                mLookAt[2] = 0.0f;

                mUp[0] = 0.0f;
                mUp[1] = 1.0f;
                mUp[2] = 0.0f;

                mTranslation[0] = 0.0f;
                mTranslation[1] = 0.0f;
                mTranslation[2] = 0.0f;
            }

            Camera::~Camera() {

            }

            void Camera::setLookAt(float x, float y, float z) {
                mLookAt[DSG_CAM_X] = x;
                mLookAt[DSG_CAM_Y] = y;
                mLookAt[DSG_CAM_Z] = z;
            }

            void Camera::setTranslation(float x, float y, float z) {
                mTranslation[DSG_CAM_X] = x;
                mTranslation[DSG_CAM_Y] = y;
                mTranslation[DSG_CAM_Z] = z;
            }

            void Camera::setUp(float x, float y, float z) {
                mUp[DSG_CAM_X] = x;
                mUp[DSG_CAM_Y] = y;
                mUp[DSG_CAM_Z] = z;
            }
        }
    }
}
