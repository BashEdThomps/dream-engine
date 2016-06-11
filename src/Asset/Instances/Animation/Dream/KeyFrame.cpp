#include "KeyFrame.h"
#include "AnimationInstance.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Animation {
namespace Dream     {
	
	KeyFrame::KeyFrame(long duration) {
	  mDuration = duration;
	}

	KeyFrame::~KeyFrame() {}

	long KeyFrame::getNumPlaybackFramesToGenerate() {
		return (long) ((mDuration/1000.0f) * AnimationInstance::getFramesPerSecond());
	}

	bool KeyFrame::getWrap() {
	    return mWrap;
	}
	
	std::vector<Frame*> KeyFrame::getPlaybackFrames() {
		return mPlaybackFrames;
	}
	
} // End of Dream
} // End of Animation
} // End of Instances
} // End of Asset
} // End of Dream
