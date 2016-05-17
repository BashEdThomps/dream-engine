#include <iostream>
#include "KeyFrame.h"
#include "FrameDelta.h"
#include "AnimationInstance.h"

namespace Dream {
namespace Asset {
namespace Instance {
namespace Animation {
	
	AnimationInstance::AnimationInstance(int fps) : Dream::Asset::Instance::AssetInstance() {
		mFramesPerSecond = fps;
	    mCurrentFrame    = 0;
	    mCurrentKeyFrame = 0;
	}

	AnimationInstance::~AnimationInstance() {}

	void AnimationInstance::generateFrames() {
		int currentKeyFrame = 0;
		int currentFrame = 0;

		while (true) {
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

            for (int i = 0; i < intermediates; i++) {
				Frame *frame = new Frame(currentFrame);
                Animation::FrameDelta *nextDelta;
                int max = frame->getNumFrameDeltas();
                for (int j=0;i<max;j++) { //FrameDelta d : source.getDeltas()) {
					nextDelta = frame->getFrameDeltas()[j];
                    std::cout << "Creatng delta form " <<  nextDelta->getDrawableID() << std::endl;
                    // TODO - FIX THIS
                    //FrameDelta* dest = NULL;// = KeyFrameDeltaGetDrawaltarget->getDeltaByDrawableID(d.getDrawableID());
                    FrameDelta* moveBy = NULL;// = KeyFrameAnimationComputeMotionDelta(nextDelta, dest, intermediates, i);
                    moveBy->showStatus();
                    frame->addFrameDelta(moveBy);
                }
                addFrame(frame);
                mCurrentFrame++;
			}
		    // Move on to the next KeyFrame
			mCurrentKeyFrame++;
		}
	}

	void AnimationInstance::addFrame(Frame* f) {
	    mFrames.push_back(f);
	}

	void AnimationInstance::addKeyFrame(KeyFrame *kf) {
	    mKeyFrames.push_back(kf);
	}

	void AnimationInstance::addDrawable(int sd) {
	    mDrawables.push_back(sd);
	}

	void AnimationInstance::removeDrawable(int dID) {
	    mDrawables[dID] = -1;
	}

	void AnimationInstance::nextFrame() {
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

	    FrameDelta *nextFrameDelta;
		int max = currentFrame->getNumFrameDeltas();
	    for (int i=0; i<max; i++) {
			nextFrameDelta = currentFrame->getFrameDeltas()[i];
			if(nextFrameDelta == NULL) {
			    continue;
			}

			//SceneObject sd = getDrawableByID(d.getDrawableID());
			applyFrameDeltaToVector(nextFrameDelta,NULL,NULL);
		}

		mCurrentFrame++;
	}

	void AnimationInstance::applyFrameDeltaToVector(FrameDelta* delta, float* posVector, float* rotVector) {
		float* posDelta = delta->getPositionDelta();
		posVector[0] += posDelta[0];
		posVector[1] += posDelta[1];
		posVector[2] += posDelta[2];
		
		float* rotDelta = delta->getRotationDelta();
		rotVector[0] += rotDelta[0];
		rotVector[1] += rotDelta[1];
		rotVector[2] += rotDelta[2];
	}

	int AnimationInstance::getFramesPerSecond() {
		return mFramesPerSecond;
	}
	
} // End of AnimationInstance
} // End of Animation
} // End of Asset
} // End of Dream
