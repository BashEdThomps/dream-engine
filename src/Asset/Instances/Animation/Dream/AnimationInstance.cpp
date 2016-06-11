
#include "AnimationInstance.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Animation {
namespace Dream     {
	
	int AnimationInstance::FramesPerSecond = 60;
	
	void AnimationInstance::setFramesPerSecond(int fps) {
		FramesPerSecond = fps;
	}
	
	int AnimationInstance::getFramesPerSecond() {
		return FramesPerSecond;
	}
	
	AnimationInstance::AnimationInstance(AssetDefinition* definition) : AssetInstance(definition) {
	  mCurrentPlaybackFrame = 0;
		mLoop                 = false;
	}

	AnimationInstance::~AnimationInstance() {}
	
	bool AnimationInstance::load(std::string projectPath) {
		return false;
	}
	
	void AnimationInstance::addKeyFrame(KeyFrame *kf) {
		mKeyFrames.push_back(kf);
	}
	
	void AnimationInstance::generatePlaybackFrames() {
		for (int keyFrame = 0; keyFrame < mKeyFrames.size(); keyFrame++) {
			KeyFrame* currentKeyFrame = mKeyFrames[keyFrame];
			KeyFrame* nextKeyFrame = NULL;
			if (keyFrame == mKeyFrames.size()-1) {
				if (currentKeyFrame->getWrap()) {
					nextKeyFrame = mKeyFrames[0];
				} else {
					break;
				}
			}
			currentKeyFrame->generateFrames(nextKeyFrame);
			std::vector<Frame*> frames = currentKeyFrame->getFrames();
			mPlaybackFrames.insert(mPlaybackFrames.end(),frames.begin(),frames.end());
		}
	}
	
	void AnimationInstance::play() {
		
	}
	
	void AnimationInstance::pause() {
		
	}
	
	void AnimationInstance::stop() {
		
	}
	
} // End of Dream
} // End of Animation
} // End of Instances
} // End of Asset
} // End of Dream
