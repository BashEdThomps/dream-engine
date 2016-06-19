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
		long                mStartTimeMS;
		bool                mWrap;
		std::vector<Frame*> mPlaybackFrames;
		std::vector<float>  mTranslation;
		std::vector<float>  mRotation;
		std::vector<float>  mScale;
		std::string         mName;
		std::string         mUUID;
	public:
		KeyFrame();
		~KeyFrame();
		void showStatus();
		std::vector<Frame*> getPlaybackFrames();
		void                generatePlaybackFrames(KeyFrame*);
		void                addPlaybackFrame(Frame*);
		bool                getWrap();
		void                setWrap(bool);
		long                getStartTimeMS();
		void                setStartTimeMS(long);
		std::vector<float>  getTranslation();
		void                setTranslation(std::vector<float>);
		std::vector<float>  getRotation();
		void                setRotation(std::vector<float>);
		std::vector<float>  getScale();
		void                setScale(std::vector<float>);
		std::string getName();
		void setName(std::string);
		std::string getUUID();
		void setUUID(std::string);
	};
	
} // End of Dream
} // End of Animation
} // End of Instances
} // End of Asset
} // End of Dream

#endif // KEYFRAME_H
