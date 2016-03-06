#ifndef DSGCAMERA_H
#define DSGCAMERA_H

#include <stdlib.h>

#define DSG_CAM_LOOK_AT_SZ     3
#define DSG_CAM_UP_SZ          3
#define DSG_CAM_TRANSLATION_SZ 3

typedef struct {
    float *lookAt;
    float *up;
    float *translation;
} dsgCamera;

dsgCamera* dsgCameraInit();

#endif  // DSGCAMERA_H
