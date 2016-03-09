#ifndef DA_KEYFRAMEANIMATION_H
#define DA_KEYFRAMEANIMATION_H

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
    int          done;

} daKeyFrameAnimation;

daKeyFrameAnimation* daKeyFrameAnimationCreate()  {
    daKeyFrameAnimation* kfa = (daKeyFrameAnimation*) malloc(sizeof(daKeyFrameAnimation));
    kfa->keyFrames           = (daKeyFrame**) malloc(sizeof(daKeyFrame*) * DA_KEYFRAMEANIM_KEYFRAMES_SZ);
    kfa->frames              = (daFrame**)    malloc(sizeof(daFrame*)    * DA_KEYFRAMEANIM_FRAMES_SZ   );
    kfa->drawables           = (int*)         malloc(sizeof(int)         * DA_KEYFRAMEANIM_DRAWABLES_SZ);
    kfa->currentFrame        = 0;
    return kfa;
}

void daKeyFrameAnimationGenerateFrames() {
        int currentKeyFrame = 0;
        int currentFrame = 0;

        while (true) {
            daKeyFrame* source = getKeyFrame(currentKeyFrame);
            daKeyFrame* target = getKeyFrame(currentKeyFrame + 1);

            if (source == null) {
                _Tracer.debug("Finished generating frames");
                break;
            }

            if (target == null) {
                if (source->wrap) {
                    target = getKeyFrame(0);
                } else {
                    _Tracer.debug("Finished generating frames");
                    break;
                }
            }

            _Tracer.info("Generating frames for " + source.getIndex() + " >> " + (target.getIndex()));

            int intermediates = source.getIntermediates();
            _Tracer.info("\t with " + intermediates + " intermediate");
            int i;
            for (i = 0; i < intermediates; i++) {
                Frame f = new Frame(currentFrame);

                for (FrameDelta d : source.getDeltas()) {
                    //_Tracer.info("Creatng delta for " + d.getDrawableID());
                    daKeyFrameDelta* dest = target.getDeltaByDrawableID(d.getDrawableID());
                    daKeyFrameDelta* moveBy = d.getMotionDelta(d, dest, intermediates, i);
                    //moveBy.show();
                    daFrameAddMotionDelta(f,moveBy);
                }

                mFrames.add(f);
                currentFrame++;
            }
            // Move on to the next KeyFrame
            currentKeyFrame++;
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
