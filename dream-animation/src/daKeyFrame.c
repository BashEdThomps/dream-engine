#include <stdlib.h>
#include "daKeyFrame.h"

daKeyFrame* daKeyFrameCreate(int index, long duration) {
    daKeyFrame *retval = (daKeyFrame*) malloc (sizeof(daKeyFrame));
    retval->index = index;
    retval->duration = duration;
    retval->deltas = (daFrameDelta**) malloc(sizeof(daFrameDelta*)*DA_KEYFRAME_DELTAS_SZ);
    return retval;
}

void daKeyFrameAddDelta(daKeyFrame* keyFrame, daFrameDelta* frameDelta) {
    int index = daKeyFrameGetNextAvailableIndex(keyFrame);
    keyFrame->deltas[index] = frameDelta;
    return;
}

int daKeyFrameGetNextAvailableIndex(daKeyFrame* keyFrame) {
    return -1;
}

int daKeyFrameGetIntermediates(daKeyFrame* keyFrame) {
    return (int)((((float)keyFrame->duration/1000))*DA_FPS);
}

int daKeyFrameCompareIndicies(daKeyFrame* obj1, daKeyFrame* obj2) {
    return obj2->index - obj1->index;
}

daFrameDelta* daKeyFrameGetDeltaByDrawableID(daKeyFrame* keyFrame, int drawableID) {
    daFrameDelta *next = 0;
    int i;
    for (i=0;i<DA_KEYFRAME_DELTAS_SZ;i++) {
        next = keyFrame->deltas[i];
        if (next->drawableID == drawableID) {
            return next;
        }
    }
    return 0;
}
