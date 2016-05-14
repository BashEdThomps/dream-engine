#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "KeyFrame.h"
#include "Frame.h"
#include "../ResourceInstance.h"

#define RESOURCE_TYPE_ANIMATION   "Animation"
#define RESOURCE_DIR_ANIMATION    "animation"
#define RESOURCE_FORMAT_ANIMATION "animation"

namespace Dream {
  namespace Resource {
    namespace Instance {
      namespace Animation {
        class AnimationInstance : public Dream::Resource::Instance::ResourceInstance {

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

          AnimationInstance(int fps = 60);
          ~AnimationInstance();

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
      }
    }
  }
}

#endif // ANIMAITON_H
