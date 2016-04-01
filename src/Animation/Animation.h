#ifndef DA_KEYFRAMEANIMATION_H
#define DA_KEYFRAMEANIMATION_H

#include "KeyFrame.h"
#include "Frame.h"

//! TODO - Document
#define DA_KEYFRAMEANIM_KEYFRAMES_SZ 1024

//! TODO - Document
#define DA_KEYFRAMEANIM_FRAMES_SZ    1024

//! TODO - Document
#define DA_KEYFRAMEANIM_DRAWABLES_SZ 1024

//! TODO - Document
namespace Dream {
    namespace Animation {
        class Animation {
        private:
            KeyFrame *mKeyFrames[DA_KEYFRAMEANIM_KEYFRAMES_SZ];
            int       mNumKeyFrames;
            Frame    *mFrames[DA_KEYFRAMEANIM_FRAMES_SZ];
            int       mDrawables[DA_KEYFRAMEANIM_DRAWABLES_SZ];
            int       mCurrentFrame;
            int       mCurrentKeyFrame;
            bool      mDone;
        public:
            //! TODO - Document
            Animation();
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
            int getNextAvailableKeyFrameIndex();
            //! TODO - Document
            int getNextAvailableFrameIndex();
            //! TODO - Document
            int getNextAvailableDrawableIndex();
            //! TODO - Document
            void applyFrameDeltaToVector(FrameDelta*,float*,float*);
            //! TODO - Document
            bool isDone();
    	    };
    }
}

#endif // DA_KEYFRAMEANIMAITON_H
