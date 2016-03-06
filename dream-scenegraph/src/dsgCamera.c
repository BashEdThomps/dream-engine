#include "dsgCamera.h"

dsgCamera* dsgCameraInit(){
    dsgCamera* retval = (dsgCamera*)malloc(sizeof(dsgCamera));

    retval->lookAt = (float*)malloc(sizeof(float)*DSG_CAM_LOOK_AT_SZ);
    retval->lookAt[0] = 0.0f;
    retval->lookAt[1] = 0.0f;
    retval->lookAt[2] = 0.0f;

    retval->up = (float*)malloc(sizeof(float)*DSG_CAM_UP_SZ);
    retval->up[0] = 0.0f;
    retval->up[1] = 1.0f;
    retval->up[2] = 0.0f;

    retval->translation = (float*)malloc(sizeof(float)*DSG_CAM_TRANSLATION_SZ);
    retval->translation[0] = 0.0f;
    retval->translation[1] = 0.0f;
    retval->translation[2] = 0.0f;

    return retval;
}

void dsgCameraDestroy(dsgCamera* camera) {
    free(camera->lookAt);
    camera->lookAt = NULL;

    free(camera->up);
    camera->up = NULL;

    free(camera->translation);
    camera->translation = NULL;
}
void dsgCameraSetLookAt(dsgCamera* camera, float x, float y, float z) {
    if (camera == NULL || camera->lookAt == NULL) {
        return;
    }
    camera->lookAt[DSG_CAM_X] = x;
    camera->lookAt[DSG_CAM_Y] = y;
    camera->lookAt[DSG_CAM_Z] = z;
}

void dsgCameraSetTranslation(dsgCamera* camera, float x, float y, float z) {
    if (camera == NULL || camera->translation == NULL) {
        return;
    }
    camera->translation[DSG_CAM_X] = x;
    camera->translation[DSG_CAM_Y] = y;
    camera->translation[DSG_CAM_Z] = z;
}

void dsgCameraSetUp(dsgCamera* camera, float x, float y, float z) {
    if (camera == NULL || camera->up == NULL) {
        return;
    }
    camera->up[DSG_CAM_X] = x;
    camera->up[DSG_CAM_Y] = y;
    camera->up[DSG_CAM_Z] = z;
}
