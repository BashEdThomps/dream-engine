#ifndef DA_KEYFRAMEANIMATION_H
#define DA_KEYFRAMEANIMATION_H

#include <vector>
#include "KeyFrame.h"
#include "Frame.h"
#include "../Resource.h"

//! TODO - Document
namespace Dream {
    namespace Resource {
        namespace Animation {
            class Animation : public Dream::Resource::Resource {
            private:
                std::vector<KeyFrame*> mKeyFrames;
                std::vector<Frame*>    mFrames;
                std::vector<int>       mDrawables;

                int       mNumKeyFrames;
                int       mCurrentFrame;
                int       mCurrentKeyFrame;
                bool      mDone;
                int       mFramesPerSecond;
            public:
                //! TODO - Document
                Animation(int fps = 60);
                Animation(nlohmann::json);
                ~Animation();
                //! TODO - Document
                void generateFrames();
                //! TODO - Document
                void addFrame(Frame*);
                //! TODO - Document
                void addKeyFrame(KeyFrame*);
                //! TODO - Document
                void addDrawable(int);
                //! TODO - Document
                void removeDrawable(int);
                //! TODO - Document
                void nextFrame();
                //! TODO - Document
                void applyFrameDeltaToVector(FrameDelta*,float*,float*);
                //! TODO - Document
                bool isDone();
                //! TODO - Document
                int getFramesPerSecond();
        	    };
        }
    }
}

#endif // DA_KEYFRAMEANIMAITON_H
