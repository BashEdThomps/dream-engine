#ifndef DA_KEYFRAMEANIMATION_H
#define DA_KEYFRAMEANIMATION_H

#include <stdlib.h>
#include <stdio.h>
#include "daKeyFrame.h"
#include "daFrame.h"

#define DA_KEYFRAMEANIM_KEYFRAMES_SZ 1024
#define DA_KEYFRAMEANIM_FRAMES_SZ    1024
#define DA_KEYFRAMEANIM_DRAWABLES_SZ 1024

typedef struct {
    daKeyFrame **keyFrames;
    int          numKeyFrames;
    daFrame    **frames;
    int         *drawables;
    int          currentFrame;
    int          currentKeyFrame;
    int          done;

} daKeyFrameAnimation;

daKeyFrameAnimation* daKeyFrameAnimationCreate ();
void daKeyFrameAnimationGenerateFrames (daKeyFrameAnimation*);
void daKeyFrameAnimationAddFrame       (daKeyFrameAnimation*,daFrame*);
void daKeyFrameAnimationAddKeyFrame    (daKeyFrameAnimation*,daKeyFrame*);
void daKeyFrameAnimationAddDrawable    (daKeyFrameAnimation*,int);
void daKeyFrameAnimationRemoveDrawable (daKeyFrameAnimation*,int);
void daKeyFrameAnimationNextFrame      (daKeyFrameAnimation*);

int daKeyFrameAnimationGetNextAvailableKeyFrameIndex(daKeyFrameAnimation*);
int daKeyFrameAnimationGetNextAvailableFrameIndex(daKeyFrameAnimation*);
int daKeyFrameAnimationGetNextAvailableDrawableIndex(daKeyFrameAnimation*);
	
void daKeyFrameAnimationApplyFrameDeltaToVector(daFrameDelta*,float*,float*); 
	
#endif // DA_KEYFRAMEANIMAITON_H
