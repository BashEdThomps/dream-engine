
#include "AnimationInstance.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Animation {
namespace Dream     {
	
	AnimationInstance::AnimationInstance(AssetDefinition* definition, int fps) : AssetInstance(definition) {
		mFramesPerSecond = fps;
	  mCurrentFrame    = 0;
	  mCurrentKeyFrame = 0;
	}

	AnimationInstance::~AnimationInstance() {}
	
	bool AnimationInstance::load(std::string projectPath) {
		return false;
	}

	void AnimationInstance::generateFrames() {
		int currentKeyFrame = 0;
		int currentFrame = 0;

		while (true) {
			KeyFrame* source = mKeyFrames[currentKeyFrame];
			KeyFrame* target = mKeyFrames[currentKeyFrame+1];

			if (source == NULL) {
				std::cout << "Animation: Finished generating frames" << std::endl;
				break;
			}

			if (target == NULL) {
				if (source->getWrap()) {
					target = mKeyFrames[0];
				} else {
			    std::cout << "Animation: Finished generating frames" <<  std::endl;
			    break;
				}
			}

			std::cout << "Animation: Generating frames for " << source->getIndex() << " >> " << target->getIndex() << std::endl;
			int intermediates = source->getIntermediateFrameCount();
			std::cout << "\t with " << intermediates << " intermediates" << std::endl;

			for (int i = 0; i < intermediates; i++) {
				Frame *frame = new Frame(currentFrame);
				FrameDelta *nextDelta;
				int max = frame->getNumFrameDeltas();
				for (int j=0;i<max;j++) {
					nextDelta = frame->getFrameDeltas()[j];
					std::cout << "Animation: Creatng delta" << std::endl;
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

	void AnimationInstance::nextFrame() {
		// We're done
		if (mDone) {
			return;
		}

		std::cout << "Animation: Applying next Frame: " << mCurrentFrame << std::endl;
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
			applyFrameDeltaToSceneObject(nextFrameDelta);
		}
		mCurrentFrame++;
	}

	void AnimationInstance::applyFrameDeltaToSceneObject(FrameDelta* delta) {
		std::vector<float> translationDelta = delta->getPositionDelta();
		std::vector<float> soTranslation = mSceneObject->getTranslation();
		
		float tX = soTranslation[0] += translationDelta[0];
		float tY = soTranslation[1] += translationDelta[1];
		float tZ = soTranslation[2] += translationDelta[2];
		mSceneObject->setTranslation(tX, tY, tZ);
		
		std::vector<float> rotationDelta = delta->getRotationDelta();
		std::vector<float> soRotation = mSceneObject->getRotation();
		float rX = soRotation[0] += rotationDelta[0];
		float rY = soRotation[1] += rotationDelta[1];
		float rZ = soRotation[2] += rotationDelta[2];
		mSceneObject->setRotation(rX, rY, rZ);
	}

	int AnimationInstance::getFramesPerSecond() {
		return mFramesPerSecond;
	}
	
} // End of Dream
} // End of Animation
} // End of Instances
} // End of Asset
} // End of Dream
