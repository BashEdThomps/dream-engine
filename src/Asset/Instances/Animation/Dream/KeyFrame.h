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
		long                mStartTtmeMS;
		bool                mWrap;
		std::vector<Frame*> mPlaybackFrames;
		std::vector<float>  mTranslation;
		std::vector<float>  mRotation;
		std::vector<float>  mScale;
	public:
		KeyFrame(long);
		~KeyFrame();
		
		std::vector<Frame*> getPlaybackFrames();
		void                generateFrames(KeyFrame*);
		void                addPlaybackFrame(Frame*);
		
		bool                getWrap();
		long                getStartTimeMS();
		
		std::vector<float> getTranslation();
		void               setTranslation(std::vector<float>);
		
		std::vector<float> getRotation();
		void               setRotation(std::vector<float>);
		
		std::vector<float> getScale();
		void               setScale(std::vector<float>);
	};
	
} // End of Dream
} // End of Animation
} // End of Instances
} // End of Asset
} // End of Dream

#endif // KEYFRAME_H
