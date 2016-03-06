#include "dsgCamera.h"

dsgCamera* dsgCameraInit(){
    dsgCamera* retval = (dsgCamera*)malloc(sizeof(dsgCamera));

    retval->lookAt = (float*)malloc(sizeof(float)*DSG_CAM_LOOK_AT_SZ);
    retval->lookAt[0] = 0.0f;
    retval->lookAt[1] = 0.0f;
    retval->lookAt[2] = 0.0f;

    retval->up = (float*)malloc(sizeof(float)*DSG_CAM_UP_SZ);
    retval->up[0] = 0.0f;
    retval->up[1] = 0.0f;
    retval->up[2] = 0.0f;

    retval->translation = (float*)malloc(sizeof(float)*DSG_CAM_TRANSLATION_SZ);
    retval->translation[0] = 0.0f;
    retval->translation[1] = 0.0f;
    retval->translation[2] = 0.0f;

    return retval;
}
