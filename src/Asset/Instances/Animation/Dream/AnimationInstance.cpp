
#include "AnimationInstance.h"
#include <algorithm>
#include "../../../../Util/String.h"

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
		mPlaying              = false;
	}

	AnimationInstance::~AnimationInstance() {}
	
	void AnimationInstance::showStatus() {
		std::cout << "AnimationInstance:" << std::endl;
		std::cout << "\tLoop: " << Util::String::boolToYesNo(mLoop) << std::endl;
	}
	
	bool AnimationInstance::load(std::string projectPath) {
		mLoop = mDefinition->getJson()[ASSET_ATTR_LOOP];
		loadExtraAttributes(mDefinition->getJson());
		return false;
	}
	
	void AnimationInstance::addKeyFrame(KeyFrame *kf) {
		mKeyFrames.push_back(kf);
	}
	
	void AnimationInstance::generatePlaybackFrames() {
		std::cout<<"AnimationInstance: Generating Playback Frames" << std::endl;
		std::vector<KeyFrame*>::iterator keyFrameIter;
		for (keyFrameIter = mKeyFrames.begin(); keyFrameIter != mKeyFrames.end(); keyFrameIter++) {
			KeyFrame* currentKeyFrame = (*keyFrameIter);
			// Get the next KeyFrame
			KeyFrame* nextKeyFrame = NULL;
			// End of Vector?
			if (currentKeyFrame == mKeyFrames.back()) {
				std::cout << "AnimationInstance: Last KeyFrame, checking for wrap..." << std::endl;
				if (currentKeyFrame->getWrap()) {
					std::cout << "AnimationInstance: KeyFrame wraps to beginning." << std::endl;
					nextKeyFrame = (*mKeyFrames.begin());
				} else {
					std::cout << "AnimationInstance: Last KeyFrame does not wrap." << std::endl;
					break;
				}
			} else {
				nextKeyFrame = *(keyFrameIter+1);
			}
			
			currentKeyFrame->generatePlaybackFrames(nextKeyFrame);
			std::vector<Frame*> frames = currentKeyFrame->getPlaybackFrames();
			mPlaybackFrames.insert(mPlaybackFrames.end(),frames.begin(),frames.end());
		}
		std::cout<<"AnimationInstance: Finished Generating Playback Frames" << std::endl;
	}
	
	void AnimationInstance::loadExtraAttributes(nlohmann::json json) {
		showStatus();
		if (!json[ASSET_ATTR_KEYFRAMES].is_null() && json[ASSET_ATTR_KEYFRAMES].is_array()){
			nlohmann::json jsonKeyFrames = json[ASSET_ATTR_KEYFRAMES];
			std::cout << "AnimationInstance: Loading KeyFrames" << std::endl;
			for (nlohmann::json::iterator it = jsonKeyFrames.begin(); it != jsonKeyFrames.end(); ++it) {
				KeyFrame *nextKeyFrame = new KeyFrame();
				std::vector<float> translation(3), rotation(3), scale(3);
				
				translation[0] = (*it)[ASSET_ATTR_TRANSLATION][ASSET_ATTR_X];
				translation[1] = (*it)[ASSET_ATTR_TRANSLATION][ASSET_ATTR_Y];
				translation[2] = (*it)[ASSET_ATTR_TRANSLATION][ASSET_ATTR_Z];
				
				rotation[0] = (*it)[ASSET_ATTR_ROTATION][ASSET_ATTR_X];
				rotation[1] = (*it)[ASSET_ATTR_ROTATION][ASSET_ATTR_Y];
				rotation[2] = (*it)[ASSET_ATTR_ROTATION][ASSET_ATTR_Z];
				
				scale[0] = (*it)[ASSET_ATTR_SCALE][ASSET_ATTR_X];
				scale[1] = (*it)[ASSET_ATTR_SCALE][ASSET_ATTR_Y];
				scale[2] = (*it)[ASSET_ATTR_SCALE][ASSET_ATTR_Z];
				
				long startTime   = (*it)[ASSET_ATTR_START_TIME];
				bool wrap        = (*it)[ASSET_ATTR_WRAP];
				std::string interpolation = (*it)[ASSET_ATTR_INTERPOLATION];
				std::string name = (*it)[ASSET_NAME];
				std::string uuid = (*it)[ASSET_UUID];
				
				nextKeyFrame->setName(name);
				nextKeyFrame->setInterpolationType(interpolation);
				nextKeyFrame->setUUID(uuid);
				nextKeyFrame->setStartTimeMS(startTime);
				nextKeyFrame->setTranslation(translation);
				nextKeyFrame->setRotation(rotation);
				nextKeyFrame->setScale(scale);
				nextKeyFrame->setWrap(wrap);
				
				nextKeyFrame->showStatus();
				
				addKeyFrame(nextKeyFrame);
			}
			generatePlaybackFrames();
		}
	}
	
	void AnimationInstance::step(double deltaTime) {
		if (mPlaying) {
			int advanceBy = ceil(deltaTime / (1000/getFramesPerSecond()));
			if (advanceBy > MAX_FRAME_ADVANCE) return;
			mCurrentPlaybackFrame += advanceBy;
			//std::cout << "AnimationInstance: Delta time: " << deltaTime << ", Advance By: " << advanceBy <<  " frames to frame: " << mCurrentPlaybackFrame << std::endl;
			if (mCurrentPlaybackFrame > mPlaybackFrames.size()) {
				if (!mLoop) {
					std::cout << "AnimationInstance: Playback Finished" << std::endl;
					mPlaying = false;
				}
				std::cout << "AnimationInstance: Returning to Frame 0" << std::endl;
				mCurrentPlaybackFrame = 0;
			}
		}
	}
	
	void AnimationInstance::play() {
		std::cout << "AnimationInstance: Playing Animation" << getName() << std::endl;
		mPlaying = true;
	}
	
	void AnimationInstance::pause() {
		std::cout << "AnimationInstance: Pausing Animation" << getName() << std::endl;
		mPlaying = false;
	}
	
	void AnimationInstance::stop() {
		std::cout << "AnimationInstance: Stopping Animation" << getName() << std::endl;
		mPlaying = false;
		mCurrentPlaybackFrame = 0;
	}
	
	bool AnimationInstance::isLooping() {
		return mLoop;
	}

	void AnimationInstance::setLooping(bool looping) {
		mLoop = looping;
	}
	
	void AnimationInstance::applyTransform(Scene::SceneObject *sceneObject) {
		if (mPlaying && mCurrentPlaybackFrame < mPlaybackFrames.size()) {
  		Frame *currentFrame = mPlaybackFrames[mCurrentPlaybackFrame];
			currentFrame->applyToSceneObject(sceneObject);
		}
	}
	
} // End of Dream
} // End of Animation
} // End of Instances
} // End of Asset
} // End of Dream
