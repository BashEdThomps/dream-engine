#ifndef DA_KEYFRAME_H
#define DA_KEYFRAME_H

#include "daFrameDelta.h"

#define DA_KEYFRAME_DELTAS_SZ 256

typedef struct {
    int index;
    daFrameDelta** deltas;
    long duration;
    int wrap;
} daKeyFrame;

daKeyFrame*   daKeyFrameCreate                (int, long);
void          daKeyFrameAddDelta              (daKeyFrame*, daFrameDelta*);
int           daKeyFrameGetNextAvailableIndex (daKeyFrame*);
int           daKeyFrameGetIntermediates      (daKeyFrame*);
int           daKeyFrameCompareIndicies       (daKeyFrame*, daKeyFrame*);
daFrameDelta* daKeyFrameGetDeltaByDrawableID  (daKeyFrame*, int);

#endif // DA_KEYFRAME_H
