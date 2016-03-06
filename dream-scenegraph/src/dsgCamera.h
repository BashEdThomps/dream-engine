#ifndef DSGCAMERA_H
#define DSGCAMERA_H

#include <stdlib.h>

#define DSG_CAM_LOOK_AT_SZ     3
#define DSG_CAM_UP_SZ          3
#define DSG_CAM_TRANSLATION_SZ 3

#define DSG_CAM_X 0
#define DSG_CAM_Y 1
#define DSG_CAM_Z 2

typedef struct {
    float *lookAt;
    float *up;
    float *translation;
} dsgCamera;

dsgCamera* dsgCameraInit();
void       dsgCameraDestroy(dsgCamera*);

void dsgCameraSetLookAt      (dsgCamera*, float, float, float);
void dsgCameraSetUp          (dsgCamera*, float, float, float);
void dsgCameraSetTranslation (dsgCamera*, float, float, float);

#endif  // DSGCAMERA_H
