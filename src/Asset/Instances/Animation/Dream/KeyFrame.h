#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <vector>
#include "Frame.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Animation {
namespace Dream     {
	
	class KeyFrame {
	private:
		long                mDuration;
		bool                mWrap;
		std::vector<Frame*> mFrames;
		std::vector<float>  mTranslation;
		std::vector<float>  mRotation;
		std::vector<float>  mScale;
	public:
		KeyFrame(long);
		~KeyFrame();
		void                generateFrames(KeyFrame*);
		std::vector<Frame*> getFrames();
		bool                getWrap();
		long                getNumPlaybackFramesToGenerate();
	};
	
} // End of Dream
} // End of Animation
} // End of Instances
} // End of Asset
} // End of Dream

#endif // KEYFRAME_H
