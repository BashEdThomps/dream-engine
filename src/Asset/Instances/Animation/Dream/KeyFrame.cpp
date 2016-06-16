#include "KeyFrame.h"
#include "AnimationInstance.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Animation {
namespace Dream     {
	
	KeyFrame::KeyFrame(long startTimeMS) {
	  mStartTtmeMS = startTimeMS;
	}

	KeyFrame::~KeyFrame() {}

	bool KeyFrame::getWrap() {
	    return mWrap;
	}
	
	long KeyFrame::getStartTimeMS() {
		return mStartTtmeMS;
	}
	
	std::vector<Frame*> KeyFrame::getPlaybackFrames() {
		return mPlaybackFrames;
	}
	
	void KeyFrame::generateFrames(KeyFrame* toKeyFrame) {
		long keyFrameDurationMS = toKeyFrame->getStartTimeMS() - getStartTimeMS();
		long numFrames = (keyFrameDurationMS/1000.0f) * AnimationInstance::getFramesPerSecond();
		
		std::vector<float> toTranslation     = toKeyFrame->getTranslation();
		std::vector<float> toTranslationStep = std::vector<float>(3);
		toTranslationStep.push_back((mTranslation[0] - toTranslation[0]) / numFrames);
		toTranslationStep.push_back((mTranslation[1] - toTranslation[1]) / numFrames);
		toTranslationStep.push_back((mTranslation[2] - toTranslation[2]) / numFrames);
		
		std::vector<float> toScale     = toKeyFrame->getScale();
		std::vector<float> toScaleStep = std::vector<float>(3);
		toScaleStep.push_back((mScale[0] - toScale[0]) / numFrames);
		toScaleStep.push_back((mScale[1] - toScale[1]) / numFrames);
		toScaleStep.push_back((mScale[2] - toScale[2]) / numFrames);
		
		std::vector<float> toRotation     = toKeyFrame->getRotation();
		std::vector<float> toRotationStep = std::vector<float>(3);
		toRotation.push_back((mRotation[0] - toRotation[0]) / numFrames);
		toRotation.push_back((mRotation[1] - toRotation[1]) / numFrames);
		toRotation.push_back((mRotation[2] - toRotation[2]) / numFrames);
		
		
		for (int frameIndex = 0; frameIndex < numFrames;frameIndex++) {
			Frame *nextFrame = new Frame();
			
			std::vector<float> nextFrameTranslation = std::vector<float>(3);
			nextFrameTranslation.push_back(toTranslationStep[0]*frameIndex);
			nextFrameTranslation.push_back(toTranslationStep[1]*frameIndex);
			nextFrameTranslation.push_back(toTranslationStep[2]*frameIndex);
			nextFrame->setTranslation(nextFrameTranslation);
			
			std::vector<float> nextFrameRotation = std::vector<float>(3);
			nextFrameRotation.push_back(toRotationStep[0]*frameIndex);
			nextFrameRotation.push_back(toRotationStep[1]*frameIndex);
			nextFrameRotation.push_back(toRotationStep[2]*frameIndex);
			nextFrame->setRotation(nextFrameRotation);
			
			std::vector<float> nextFrameScale = std::vector<float>(3);
			nextFrameScale.push_back(toScaleStep[0]*frameIndex);
			nextFrameScale.push_back(toScaleStep[1]*frameIndex);
			nextFrameScale.push_back(toScaleStep[2]*frameIndex);
			nextFrame->setScale(nextFrameScale);
			
			addPlaybackFrame(nextFrame);
		}
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
	
} // End of Dream
} // End of Animation
} // End of Instances
} // End of Asset
} // End of Dream
