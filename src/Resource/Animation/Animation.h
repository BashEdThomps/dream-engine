#ifndef DA_KEYFRAMEANIMATION_H
#define DA_KEYFRAMEANIMATION_H

#include <vector>
#include "KeyFrame.h"
#include "Frame.h"
#include "../Resource.h"

#define RESOURCE_TYPE_ANIMATION "Animation"
#define RESOURCE_DIR_ANIMATION "animation"
#define RESOURCE_FORMAT_ANIMATION "animation"

namespace Dream {
  namespace Resource {
    namespace Animation {
      class Animation : public Dream::Resource::Resource {
      private:
        std::string mAnimationPath;
        std::vector<KeyFrame*> mKeyFrames;
        std::vector<Frame*>    mFrames;
        std::vector<int>       mDrawables;
        int       mNumKeyFrames;
        int       mCurrentFrame;
        int       mCurrentKeyFrame;
        bool      mDone;
        int       mFramesPerSecond;
      public:
        Animation(int fps = 60);
        Animation(nlohmann::json);
        ~Animation();
        void generateFrames();
        void addFrame(Frame*);
        void addKeyFrame(KeyFrame*);
        void addDrawable(int);
        void removeDrawable(int);
        void nextFrame();
        void applyFrameDeltaToVector(FrameDelta*,float*,float*);
        bool isDone();
        int getFramesPerSecond();
		    void generateAbsolutePaths(std::string,std::string);
        ResourceInstance* createInstance();
  	  };
    }
  }
}

#endif // DA_KEYFRAMEANIMAITON_H
