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
	
	std::vector<Frame*> KeyFrame::getFrames() {
		return mFrames;
	}
	
	void KeyFrame::generateFrames(KeyFrame* toKeyFrame) {
		long numFrames = toKeyFrame->getStartTimeMS() - getStartTimeMS();
		numFrames = (numFrames/1000.0f) * AnimationInstance::getFramesPerSecond();
	}
	
} // End of Dream
} // End of Animation
} // End of Instances
} // End of Asset
} // End of Dream
