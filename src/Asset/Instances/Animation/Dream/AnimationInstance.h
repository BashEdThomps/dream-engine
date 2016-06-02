#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <vector>

#include "../../../AssetInstance.h"
#include "../../../AssetDefinition.h"

#include "FrameDelta.h"
#include "KeyFrame.h"
#include "Frame.h"

#define ASSET_DIR_ANIMATION    "animation"
#define ASSET_FORMAT_ANIMATION "animation"

namespace Dream          {
namespace Asset          {
namespace Instances      {
namespace Animation      {
namespace DreamAnimation {
	
    class AnimationInstance : public AssetInstance {
    private:
        std::string            mAnimationPath;
        std::vector<KeyFrame*> mKeyFrames;
        std::vector<Frame*>    mFrames;
        std::vector<int>       mDrawables;
        int       mNumKeyFrames;
        int       mCurrentFrame;
        int       mCurrentKeyFrame;
        int       mFramesPerSecond;
        bool      mDone;
    public:
        AnimationInstance(AssetDefinition*, int fps = 60);
        ~AnimationInstance();
		
				bool load(std::string);
		
        void generateFrames();
        void addFrame(Frame*);
        void addKeyFrame(KeyFrame*);
        void addDrawable(int);
        void removeDrawable(int);
        void nextFrame();
        void applyFrameDeltaToVector(FrameDelta*,float*,float*);
        bool isDone();
        int getFramesPerSecond();
    };
	  
} // End of Dream
} // End of Animation
} // End of Instances
} // End of Asset
} // End of Dream

#endif // ANIMAITON_H
