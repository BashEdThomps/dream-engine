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

daKeyFrameAnimation* daKeyFrameAnimationCreate()  {
    daKeyFrameAnimation* kfa = (daKeyFrameAnimation*) malloc(sizeof(daKeyFrameAnimation));
    kfa->keyFrames           = (daKeyFrame**) malloc(sizeof(daKeyFrame*) * DA_KEYFRAMEANIM_KEYFRAMES_SZ);
    kfa->frames              = (daFrame**)    malloc(sizeof(daFrame*)    * DA_KEYFRAMEANIM_FRAMES_SZ   );
    kfa->drawables           = (int*)         malloc(sizeof(int)         * DA_KEYFRAMEANIM_DRAWABLES_SZ);
    kfa->currentFrame        = 0;
    kfa->currentKeyFrame     = 0;
    return kfa;
}

void daKeyFrameAnimationGenerateFrames(daKeyFrameAnimation* kfa) {
        int currentKeyFrame = 0;
        int currentFrame = 0;

        while (1) {
            daKeyFrame* source = kfa->keyFrames[currentKeyFrame];
            daKeyFrame* target = kfa->keyFrames[currentKeyFrame+1];

            if (source == NULL) {
                fprintf(stdout, "Finished generating frames\n");
                break;
            }

            if (target == NULL) {
                if (source->wrap) {
                    target = kfa->keyFrames[0];
                } else {
                    fprintf(stdout,"Finished generating frames\n");
                    break;
                }
            }

            fprintf(stdout,"Generating frames for %d >> %d\n" , source->index, target->index);

            int intermediates = daKeyFrameGetIntermediates(source);
            fprintf(stdout, "\t with %d intermediates\n", intermediates);

            int i;
            for (i = 0; i < intermediates; i++) {
                daFrame *frame = daFrameCreate(currentFrame);
                int j;
                daFrameDelta *nextDelta;
                for (j=0;i<DA_FRAME_DELTA_SZ;j++) { //FrameDelta d : source.getDeltas()) {
                    nextDelta = frame->frameDeltas[j];
                    fprintf(stdout,"Creatng delta for " + d.getDrawableID());
                    daFrameDelta* dest = target->.getDeltaByDrawableID(d.getDrawableID());
                    daFrameDelta* moveBy = d.getMotionDelta(d, dest, intermediates, i);
                    daFrameDeltaPrintDebug(moveBy);
                    daFrameAddMotionDelta(f,moveBy);
                }

                mFrames.add(f);
                kfa->currentFrame++;
            }
            // Move on to the next KeyFrame
            kfa->currentKeyFrame++;
        }
    }

void daKeyFrameAnimationAddFrame(daKeyFrameAnimation* keyFrameAnim, daFrame* f) {
    int index = daKeyFrameAnimationGetNextAvailableFrameIndex(keyFrameAnim);
    keyFrameAnim->frames[index] = f;
    mFrames.add(f);
}

void daKeyFrameAnimationAddKeyFrame(daKeyFrameAnimation *kfa, KeyFrame *kf) {
    int index = daKeyFrameAnimationGetNextAvailableKeyFrameIndex(kfa);
    kfa->keyFrames[index] = kf;
    return;
}

    public void addDrawable(SceneDrawable sd)
    {
        mDrawables.add(sd);
    }

    public void removeDrawable(int dID)
    {
        mDrawables.remove(dID);
    }

    public void drawAnimated()
    {
        for (SceneDrawable sd : mDrawables)
        {
            sd.draw();
        }
    }

void daKeyFrameAnimationNextFrame(daKeyFrameAnimation* kfa) {
    // We're done
    if (kfa->done) return;

    fprintf(stdout,"Applying next Frame: %d\n" , kfa->currentFrame);
    daFrame *currentFrame = kfa->frames[kfa->currentFrame];

    if (currentFrame == NULL) {
        if (kfa->keyFrames[kfa->numKeyFrames]->wrap) {
            kfa->currentFrame = 0;
        } else
        {
            kfa->done = true;
            return;
        }
        //return;
    }
    for (FrameDelta d : currentFrame.getMotionDeltas())
    {
        SceneObject sd = getDrawableByID(d.getDrawableID());
        sd.applyDelta(d);
    }
    kfa->currentFrame++;
}

#endif // DA_KEYFRAMEANIMAITON_H
