#ifndef DA_KEYFRAMEANIMATION_H
#define DA_KEYFRAMEANIMATION_H

#include <stdlib.h>
#include <stdio.h>
#include "daKeyFrame.h"
#include "daFrame.h"

//! TODO - Document
#define DA_KEYFRAMEANIM_KEYFRAMES_SZ 1024

//! TODO - Document
#define DA_KEYFRAMEANIM_FRAMES_SZ    1024

//! TODO - Document
#define DA_KEYFRAMEANIM_DRAWABLES_SZ 1024

//! TODO - Document
typedef struct {
    daKeyFrame **keyFrames;
    int          numKeyFrames;
    daFrame    **frames;
    int         *drawables;
    int          currentFrame;
    int          currentKeyFrame;
    int          done;
} daKeyFrameAnimation;

//! TODO - Document
daKeyFrameAnimation* daKeyFrameAnimationCreate ();
//! TODO - Document
void daKeyFrameAnimationGenerateFrames (daKeyFrameAnimation*);
//! TODO - Document
void daKeyFrameAnimationAddFrame       (daKeyFrameAnimation*,daFrame*);
//! TODO - Document
void daKeyFrameAnimationAddKeyFrame    (daKeyFrameAnimation*,daKeyFrame*);
//! TODO - Document
void daKeyFrameAnimationAddDrawable    (daKeyFrameAnimation*,int);
//! TODO - Document
void daKeyFrameAnimationRemoveDrawable (daKeyFrameAnimation*,int);
//! TODO - Document
void daKeyFrameAnimationNextFrame      (daKeyFrameAnimation*);

//! TODO - Document
int daKeyFrameAnimationGetNextAvailableKeyFrameIndex(daKeyFrameAnimation*);
//! TODO - Document
int daKeyFrameAnimationGetNextAvailableFrameIndex(daKeyFrameAnimation*);
//! TODO - Document
int daKeyFrameAnimationGetNextAvailableDrawableIndex(daKeyFrameAnimation*);
	
//! TODO - Document
void daKeyFrameAnimationApplyFrameDeltaToVector(daFrameDelta*,float*,float*); 

#endif // DA_KEYFRAMEANIMAITON_H
