#include <stdlib.h>
#include "daFrame.h"

daFrame* daFrameCreate(int index) {
    daFrame *retval = (daFrame*)malloc(sizeof(daFrame));
    retval->index = index;
    int i;
    for (i=0;i<DA_FRAME_DELTA_SZ;i++) {
	    retval->frameDeltas[i] = NULL;
    }
    return retval;
}

void daFrameAddFrameDelta(daFrame* frame, daFrameDelta* frameDelta) {
    int index = daFrameGetNextAvailableFrameDeltaIndex(frame);
    frame->frameDeltas[index] = frameDelta;
    return;
}

int daFrameGetNextAvailableFrameDeltaIndex(daFrame* frame) {
	int i;
	int retval = 0;
	for (i=0; i<DA_FRAME_DELTA_SZ; i++) {
		if(frame->frameDeltas[i] == NULL) {
			retval = i;
			break;
		}
	}
	return retval;
}

int daFrameCompareIndecies(daFrame* frame, daFrame* frame2) {
    return frame2->index - frame->index;
}


int daFrameGetNumFrameDeltas (daFrame* frame) {
	int i;
	int retval = 0;
	for (i=0;i<DA_FRAME_DELTA_SZ;i++) {
		if (frame->frameDeltas[i] != NULL) {
			retval++;
		}
	}
	return retval;
}


