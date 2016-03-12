#ifndef DA_FRAME_H
#define DA_FRAME_H

#define DA_FRAME_DELTA_SZ 1024

#include "daFrameDelta.h"

//! TODO - Document
typedef struct {
    int           index;
    daFrameDelta* frameDeltas[DA_FRAME_DELTA_SZ];
} daFrame;

//! TODO - Document
daFrame *daFrameCreate                          (int);
//! TODO - Document
void     daFrameAddFrameDelta                   (daFrame*, daFrameDelta*);
//! TODO - Document
int      daFrameGetNextAvailableFrameDeltaIndex (daFrame*);
//! TODO - Document
int      daFrameCompareIndecies                 (daFrame*, daFrame*);
//! TODO - Document
int      daFrameGetNumFrameDeltas               (daFrame*);

#endif // DA_FRAME
