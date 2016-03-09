#include "daFrame.h"

daFrame* daFrameCreate(int index) {
    daFrame *retval = (daFrame*)malloc(sizeof(daFrame));
    retval->index = index;
    return retval;
}

void daFrameAddMotionDelta(daFrame* frame, daFrameDelta* frameDelta) {
    int index = daFrameGetNextAvailableFrameDeltaIndex(frame);
    frame->frameDeltas[index] = frameDelta;
    return;
}

int daFrameGetNextAvailableFrameDeltaIndex(daFrame* frame) {
    return -1;
}

int daFrameGetIndex(daFrame* frame) {
    return frame->index;
}

int daFrameCompareIndecies(daFrame* frame, daFrame* frame2) {
    return frame2->index - frame->index;
}
