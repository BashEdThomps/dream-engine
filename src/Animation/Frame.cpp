#include <iostream>
#include "Frame.h"

namespace Dream {
    namespace Animation {
        Frame::Frame(int index) {
            mIndex = index;
            int i;
            for (i=0;i<DA_FRAME_DELTA_SZ;i++) {
        	    mFrameDeltas[i] = NULL;
            }
        }

        void  Frame::addFrameDelta(FrameDelta* frameDelta) {
            int index =  getNextAvailableFrameDeltaIndex();
            mFrameDeltas[index] = frameDelta;
            return;
        }

        int Frame::getNextAvailableFrameDeltaIndex() {
        	int i;
        	int retval = 0;
        	for (i=0; i<DA_FRAME_DELTA_SZ; i++) {
        		if(mFrameDeltas[i] == NULL) {
        			retval = i;
        			break;
        		}
        	}
        	return retval;
        }

        int Frame::compareIndecies(Frame *frame2) {
            return frame2->getIndex() - getIndex();
        }


        int Frame::getNumFrameDeltas() {
        	int i;
        	int retval = 0;
        	for (i=0;i<DA_FRAME_DELTA_SZ;i++) {
        		if (mFrameDeltas[i] != NULL) {
        			retval++;
        		}
        	}
        	return retval;
        }

        int Frame::getIndex() {
            return mIndex;
        }

        FrameDelta** Frame::getFrameDeltas() {
    	    return mFrameDeltas;
        }
    }
}
