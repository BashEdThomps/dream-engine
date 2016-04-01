#include <iostream>
#include "KeyFrame.h"
#include "FrameDelta.h"
#include "Animation.h"

namespace Dream {
	namespace Resource {
		namespace Animation {
			Animation::Animation()  {
			    mCurrentFrame    = 0;
			    mCurrentKeyFrame = 0;
			}

			Animation::~Animation() {

			}

			void Animation::generateFrames() {
				int currentKeyFrame = 0;
				int currentFrame = 0;

				while (1) {
				    KeyFrame* source = mKeyFrames[currentKeyFrame];
				    KeyFrame* target = mKeyFrames[currentKeyFrame+1];

				    if (source == NULL) {
					std::cout << "Finished generating frames" << std::endl;
					break;
				    }

				    if (target == NULL) {
					if (source->getWrap()) {
					    target = mKeyFrames[0];
					} else {
					    std::cout << "Finished generating frames" <<  std::endl;
					    break;
					}
				    }

				    std::cout << "Generating frames for " << source->getIndex() << " >> " << target->getIndex() << std::endl;

				    int intermediates = source->getIntermediateFrameCount();
				    std::cout << "\t with " << intermediates << " intermediates" << std::endl;

				    int i;
				    for (i = 0; i < intermediates; i++) {
					Frame *frame = new Frame(currentFrame);
					int j;
					FrameDelta *nextDelta;
					for (j=0;i<DA_FRAME_DELTA_SZ;j++) { //FrameDelta d : source.getDeltas()) {
					    nextDelta = frame->getFrameDeltas()[j];
					    std::cout << "Creatng delta form " <<  nextDelta->getDrawableID() << std::endl;
					    // TODO - FIX THIS
					    FrameDelta* dest;// = KeyFrameDeltaGetDrawaltarget->getDeltaByDrawableID(d.getDrawableID());
					    FrameDelta* moveBy;// = KeyFrameAnimationComputeMotionDelta(nextDelta, dest, intermediates, i);
					    moveBy->printDebug();
					    frame->addFrameDelta(moveBy);
					}
					addFrame(frame);
					mCurrentFrame++;
				    }
				    // Move on to the next KeyFrame
				    mCurrentKeyFrame++;
				}
			    }

			void Animation::addFrame(Frame* f) {
			    int index = getNextAvailableFrameIndex();
			    mFrames[index] = f;
			}

			void Animation::addKeyFrame(KeyFrame *kf) {
			    int index = getNextAvailableKeyFrameIndex();
			    mKeyFrames[index] = kf;
			    return;
			}

			int Animation::getNextAvailableKeyFrameIndex() {
				int retval = -1;
				int i;
				KeyFrame* next = NULL;
				for(i=0;i<DA_KEYFRAMEANIM_FRAMES_SZ;i++){
					next=mKeyFrames[i];
					if(next == NULL) {
						retval = i;
						break;
					}
				}
				return retval;
			}

			int Animation::getNextAvailableFrameIndex() {
				int retval = -1;
				int i;
				Frame* next = NULL;
				for(i=0;i<DA_KEYFRAMEANIM_FRAMES_SZ;i++){
					next=mFrames[i];
					if(next == NULL) {
						retval = i;
						break;
					}
				}
				return retval;
			}

			int Animation::getNextAvailableDrawableIndex() {
				int retval = -1;
				int i;
				int next = -1;
				for(i=0;i<DA_KEYFRAMEANIM_FRAMES_SZ;i++){
					next=mDrawables[i];
					if(next == -1) {
						retval = i;
						break;
					}
				}
				return retval;
			}
			void Animation::addDrawable(int sd) {
			    int index =  getNextAvailableDrawableIndex();
			    mDrawables[index] = sd;
			    return;
			}

			void Animation::removeDrawable(int dID) {
			    mDrawables[dID] = -1;
			    return;
			}

			void Animation::nextFrame() {
			    // We're done
			    if (mDone) {
				return;
			    }

			    std::cout << "Applying next Frame: " << mCurrentFrame << std::endl;
			    Frame *currentFrame = mFrames[mCurrentFrame];

			    if (currentFrame == NULL) {
				if (mKeyFrames[mNumKeyFrames]->getWrap()) {
				    mCurrentFrame = 0;
				} else {
				    mDone = 1;
				    return;
				}
			    }

			    int i;
			    FrameDelta *nextFrameDelta;
			    for (i=0; i<DA_FRAME_DELTA_SZ; i++) {
				nextFrameDelta = currentFrame->getFrameDeltas()[i];
				if(nextFrameDelta == NULL) {
				    continue;
				}

				//SceneObject sd = getDrawableByID(d.getDrawableID());
				applyFrameDeltaToVector(nextFrameDelta,NULL,NULL);
			    }

			    mCurrentFrame++;
			}

			void Animation::applyFrameDeltaToVector(
					FrameDelta* delta,
					float* posVector,
					float* rotVector) {
				posVector[DA_X] += delta->getPositionDelta()[DA_X];
				posVector[DA_Y] += delta->getPositionDelta()[DA_Y];
				posVector[DA_Z] += delta->getPositionDelta()[DA_Z];

				rotVector[DA_X] += delta->getRotationDelta()[DA_X];
				rotVector[DA_Y] += delta->getRotationDelta()[DA_Y];
				rotVector[DA_Z] += delta->getRotationDelta()[DA_Z];
			}

		}
	}
}
