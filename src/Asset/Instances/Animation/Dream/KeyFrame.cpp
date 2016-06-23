#include "KeyFrame.h"
#include "AnimationInstance.h"
#include "../../../../Util/String.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Animation {
namespace Dream     {
	
	KeyFrame::KeyFrame() {
		mInterpolationType = DREAM_ANIMATION_INTERPOLATION_NONE;
	}
	
	KeyFrame::~KeyFrame() {}

	bool KeyFrame::getWrap() {
		return mWrap;
	}
	
	long KeyFrame::getStartTimeMS() {
		return mStartTimeMS;
	}
	
	std::vector<Frame*> KeyFrame::getPlaybackFrames() {
		return mPlaybackFrames;
	}
	
	void KeyFrame::generatePlaybackFrames(KeyFrame* toKeyFrame) {
		std::cout << "KeyFrame: Generating " << mInterpolationType << " playback frames from "
		          << mName << " to " << toKeyFrame->getName() << std::endl;
		long keyFrameDurationMS = toKeyFrame->getStartTimeMS() - getStartTimeMS();
		long numFrames = (keyFrameDurationMS/1000.0f) * AnimationInstance::getFramesPerSecond();
		
		if (isInterpolationTypeNone()) {
				generateNoneInterpolationFrames(toKeyFrame,numFrames);
		} else if (isInterpolationTypeLinear()) {
				generateLinearInterpolationFrames(toKeyFrame,numFrames);
		} else if (isInterpolationTypeBezier()) {
				generateBezierInterpolationFrames(toKeyFrame,numFrames);
		}
	}
	
	bool KeyFrame::isInterpolationTypeNone() {
		return mInterpolationType.compare(DREAM_ANIMATION_INTERPOLATION_NONE) == 0;
	}
						 
	bool KeyFrame::isInterpolationTypeLinear() {
		return mInterpolationType.compare(DREAM_ANIMATION_INTERPOLATION_LINEAR) == 0;
	}
	
	bool KeyFrame::isInterpolationTypeBezier() {
		return mInterpolationType.compare(DREAM_ANIMATION_INTERPOLATION_BEZIER) == 0;
	}
	
	void KeyFrame::generateNoneInterpolationFrames(KeyFrame* toKeyFrame, long numFrames) {
		for (int frameIndex = 0; frameIndex < numFrames; frameIndex++) {
			Frame *nextFrame = new Frame();
			nextFrame->setTranslation(mTranslation);
			nextFrame->setRotation(mRotation);
			nextFrame->setScale(mScale);
			nextFrame->showStatus();
			addPlaybackFrame(nextFrame);
		}
	}
	
	void KeyFrame::generateLinearInterpolationFrames(KeyFrame* toKeyFrame, long numFrames) {
		std::vector<float> toTranslation     = toKeyFrame->getTranslation();
		std::vector<float> toTranslationStep = std::vector<float>(3);
		toTranslationStep[0] = ((toTranslation[0] - mTranslation[0]) / numFrames);
		toTranslationStep[1] = ((toTranslation[1] - mTranslation[1]) / numFrames);
		toTranslationStep[2] = ((toTranslation[2] - mTranslation[2]) / numFrames);
		
		std::vector<float> toRotation     = toKeyFrame->getRotation();
		std::vector<float> toRotationStep = std::vector<float>(3);
		toRotationStep[0] = ((toRotation[0] - mRotation[0]) / numFrames);
		toRotationStep[1] = ((toRotation[1] - mRotation[1]) / numFrames);
		toRotationStep[2] = ((toRotation[2] - mRotation[2]) / numFrames);
			
		std::vector<float> toScale     = toKeyFrame->getScale();
		std::vector<float> toScaleStep = std::vector<float>(3);
		toScaleStep[0] = ((toScale[0] - mScale[0]) / numFrames);
		toScaleStep[1] = ((toScale[1] - mScale[1]) / numFrames);
		toScaleStep[2] = ((toScale[2] - mScale[2]) / numFrames);
	
		for (int frameIndex = 0; frameIndex < numFrames; frameIndex++) {
			Frame *nextFrame = new Frame();
			
			std::vector<float> nextFrameTranslation = std::vector<float>(3);
			nextFrameTranslation[0] = mTranslation[0] + (toTranslationStep[0]*frameIndex);
			nextFrameTranslation[1] = mTranslation[1] + (toTranslationStep[1]*frameIndex);
			nextFrameTranslation[2] = mTranslation[2] + (toTranslationStep[2]*frameIndex);
			nextFrame->setTranslation(nextFrameTranslation);
			
			std::vector<float> nextFrameRotation = std::vector<float>(3);
			nextFrameRotation[0] = mRotation[0] + (toRotationStep[0]*frameIndex);
			nextFrameRotation[1] = mRotation[1] + (toRotationStep[1]*frameIndex);
			nextFrameRotation[2] = mRotation[2] + (toRotationStep[2]*frameIndex);
			nextFrame->setRotation(nextFrameRotation);
			
			std::vector<float> nextFrameScale = std::vector<float>(3);
			nextFrameScale[0] = mScale[0] + (toScaleStep[0]*frameIndex);
			nextFrameScale[1] = mScale[1] + (toScaleStep[1]*frameIndex);
			nextFrameScale[2] = mScale[2] + (toScaleStep[2]*frameIndex);
			nextFrame->setScale(nextFrameScale);
			nextFrame->showStatus();
			addPlaybackFrame(nextFrame);
		}
	}
	
	void KeyFrame::generateBezierInterpolationFrames(KeyFrame* toKeyFrame, long numFrames) {
		std::cerr << "KeyFrame: generateBezierInterpolationFrames is not implemented!" << std::endl;
	}
	
	void KeyFrame::addPlaybackFrame(Frame* frame) {
		mPlaybackFrames.push_back(frame);
	}
	
	std::vector<float> KeyFrame::getTranslation() {
		return mTranslation;
	}
	
	std::vector<float> KeyFrame::getRotation() {
		return mRotation;
	}
	
	std::vector<float> KeyFrame::getScale() {
		return mScale;
	}
	
	void KeyFrame::setRotation(std::vector<float> rotation) {
		mRotation = rotation;
	}
	
	void KeyFrame::setScale(std::vector<float> scale) {
		mScale = scale;
	}
	
	void KeyFrame::setTranslation(std::vector<float> translation) {
		mTranslation = translation;
	}
	
	void KeyFrame::setWrap(bool wrap) {
		mWrap = wrap;
	}
	
	void KeyFrame::setStartTimeMS(long startTime) {
		mStartTimeMS = startTime;
	}
	
	std::string KeyFrame::getName() {
		return mName;
	}
	
	void KeyFrame::setName(std::string name) {
		mName = name;
	}
	
	std::string KeyFrame::getUUID(){
		return mUUID;
	}
	
	void KeyFrame::setUUID(std::string uuid) {
		mUUID = uuid;
	}
	
	void KeyFrame::showStatus() {
		std::cout << "KeyFrame:" << std::endl
		          << "\t         UUID: " << mUUID << std::endl
		          << "\t         Name: " << mName << std::endl
		          << "\t  Translation: " << Util::String::floatVectorToString(mTranslation)<< std::endl
		          << "\t     Rotation: " << Util::String::floatVectorToString(mRotation)<< std::endl
		          << "\t        Scale: " << Util::String::floatVectorToString(mScale) << std::endl
		          << "\t   Start Time: " << mStartTimeMS << std::endl
		          << "\t         Wrap: " << Util::String::boolToYesNo(mWrap) << std::endl
							<< "\tInterpolation: " << getInterpolationType()
		          << std::endl;
	}
	
	void KeyFrame::setInterpolationType(std::string interpolationType) {
		mInterpolationType = interpolationType;
	}
	
	std::string KeyFrame::getInterpolationType() {
		return mInterpolationType;
	}
	
} // End of Dream
} // End of Animation
} // End of Instances
} // End of Asset
} // End of Dream
