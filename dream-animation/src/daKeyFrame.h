#ifndef DA_KEYFRAME_H
#define DA_KEYFRAME_H

#include "daFrameDelta.h"

//! TODO - Document
#define DA_KEYFRAME_DELTAS_SZ 256

//! TODO - Document
typedef struct {
    int index;
    daFrameDelta** deltas;
    long duration;
    int wrap;
} daKeyFrame;

//! TODO - Document
daKeyFrame*   daKeyFrameCreate                (int, long);
//! TODO - Document
void          daKeyFrameAddDelta              (daKeyFrame*, daFrameDelta*);
//! TODO - Document
int           daKeyFrameGetNextAvailableIndex (daKeyFrame*);
//! TODO - Document
int           daKeyFrameGetIntermediates      (daKeyFrame*);
//! TODO - Document
int           daKeyFrameCompareIndicies       (daKeyFrame*, daKeyFrame*);
//! TODO - Document
daFrameDelta* daKeyFrameGetDeltaByDrawableID  (daKeyFrame*, int);

#endif // DA_KEYFRAME_H
