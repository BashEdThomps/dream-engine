#include "Camera.h"

DreamScenegraph::Camera() {
    mLookAt[0] = 0.0f;
    mLookAt[1] = 0.0f;
    mLookAt[2] = 0.0f;

    retval->up[0] = 0.0f;
    retval->up[1] = 1.0f;
    retval->up[2] = 0.0f;

    mTranslation[0] = 0.0f;
    mTranslation[1] = 0.0f;
    mTranslation[2] = 0.0f;
}

DreamScenegraph::~Camera(dsgCamera* camera) {
    if (mLookAt != NULL) {
        delete mLookAt;
    }

    if (mUp != NULL) {
        delete mUp;
    }

    if (mTranslation != NULL ) {
        delete mTranslation;
    }
}

void DreamScenegraph::Camera::setLookAt(float x, float y, float z) {
    mLookAt[DSG_CAM_X] = x;
    mLookAt[DSG_CAM_Y] = y;
    mLookAt[DSG_CAM_Z] = z;
}

void DreamScenegraph::Camera::setTranslation(float x, float y, float z) {
    mTranslation[DSG_CAM_X] = x;
    mTranslation[DSG_CAM_Y] = y;
    mTranslation[DSG_CAM_Z] = z;
}

void DreamScenegraph::Camera::setUp(float x, float y, float z) {
    mUp[DSG_CAM_X] = x;
    mUp[DSG_CAM_Y] = y;
    mUp[DSG_CAM_Z] = z;
}
