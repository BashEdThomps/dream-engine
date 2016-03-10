#ifndef DA_FRAME_H
#define DA_FRAME_H

#define DA_FRAME_DELTA_SZ 1024

#include "daFrameDelta.h"

typedef struct {
    int           index;
    daFrameDelta* frameDeltas[DA_FRAME_DELTA_SZ];
} daFrame;

daFrame *daFrameCreate                          (int);
void     daFrameAddMotionDelta                  (daFrame*, daFrameDelta*);
int      daFrameGetNextAvailableFrameDeltaIndex (daFrame*);
int      daFrameGetIndex                        (daFrame* frame);
int      daFrameCompareIndecies                 (daFrame*, daFrame*);
int      daFrameGetNumFrameDeltas               (daFrame*);

#endif // DA_FRAME
