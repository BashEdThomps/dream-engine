#include "KeyFrame.h"

namespace Dream {
	namespace Resource {
		namespace Instance {
			namespace Animation {
				KeyFrame::KeyFrame(int fps, int index, long duration) {
					mFramesPerSecond = fps;
				    mIndex = index;
				    mDuration = duration;
				}

				KeyFrame::~KeyFrame() {

				}

				void KeyFrame::addDelta(FrameDelta* frameDelta) {
				    mDeltas.push_back(frameDelta);
				    return;
				}

				int KeyFrame::getIntermediateFrameCount() {
				    return (int)((((float)mDuration/1000))*mFramesPerSecond);
				}

				int KeyFrame::compareIndicies(KeyFrame* obj2) {
				    return obj2->getIndex() - getIndex();
				}

				int KeyFrame::getNumberOfFrameDeltas() {
					return mDeltas.size();
				}

				FrameDelta* KeyFrame::getDeltaByDrawableID(int drawableID) {
				    FrameDelta *next = 0;
				    int i;
					int max = getNumberOfFrameDeltas();
				    for (i=0;i<max;i++) {
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
		}
	}
}
