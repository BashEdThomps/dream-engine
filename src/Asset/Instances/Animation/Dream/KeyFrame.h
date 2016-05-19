#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <vector>
#include "FrameDelta.h"

namespace Dream          {
namespace Asset          {
namespace Instances      {
namespace Animation      {
namespace DreamAnimation {
	
	class KeyFrame {
	private:
		int mIndex;
		std::vector<FrameDelta*> mDeltas;
		long mDuration;
		int mWrap;
		int mFramesPerSecond;
	public:
		KeyFrame(int, int, long);
		~KeyFrame();
		void addDelta(FrameDelta*);
		int getNumberOfFrameDeltas();
		int getIntermediateFrameCount();
		int compareIndicies(KeyFrame*);
		FrameDelta* getDeltaByDrawableID(int);
		int getWrap();
		int getIndex();
	};
	
} // End of DreamAnimation
} // End of Animation
} // End of Instances
} // End of Asset
} // End of Dream

#endif // KEYFRAME_H
