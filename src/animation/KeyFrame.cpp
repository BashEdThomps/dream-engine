#include "KeyFrame.h"

namespace DreamAnimation {
	
	KeyFrame::KeyFrame(int index, long duration) {
	    mIndex = index;
	    mDuration = duration;
	}

	KeyFrame::~KeyFrame() {

	}

	void KeyFrame::addDelta(FrameDelta* frameDelta) {
	    int index = getNextAvailableIndex();
	    mDeltas[index] = frameDelta;
	    return;
	}

	int KeyFrame::getNextAvailableIndex() {
	    return -1;
	}

	int KeyFrame::getIntermediateFrameCount() {
	    return (int)((((float)mDuration/1000))*DA_FPS);
	}

	int KeyFrame::compareIndicies(KeyFrame* obj2) {
	    return obj2->getIndex() - getIndex();
	}

	FrameDelta* KeyFrame::getDeltaByDrawableID(int drawableID) {
	    FrameDelta *next = 0;
	    int i;
	    for (i=0;i<DA_DELTA_SZ;i++) {
		next = mDeltas[i];
		if (next->getDrawableID() == drawableID) {
		    return next;
		}
	    }
	    return 0;
	}

	int KeyFrame::getWrap() {
	    return mWrap;
	}
	
	int KeyFrame::getIndex() {
	    return mIndex;
	}
}
