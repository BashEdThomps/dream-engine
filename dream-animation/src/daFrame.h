#ifndef DA_FRAME_H
#define DA_FRAME_H

#define DA_FRAME_DELTA_SZ 64

#include "daFrameDelta.h"

typedef struct {
    int           index;
    daFrameDelta *frameDeltas[DA_FRAME_DELTA_SZ];
} daFrame;

daFrame *daFrameInit(int);
void daFrameAddMotionDelta(daFrame*, daFrameDelta*);
int daFrameGetNextAvailableFrameDeltaIndex(daFrame*);
int daFrameGetIndex(daFrame* frame);
int daFrameCompareIndecies(daFrame*, daFrame*);

#endif // DA_FRAME
