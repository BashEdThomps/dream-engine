#include "daKeyFrame.h"
#include "daFrameDelta.h"
#include "daKeyFrameAnimation.h"

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
                    fprintf(stdout,"Creatng delta for %d\n" , nextDelta->drawableID);
		    // TODO - FIX THIS 
                    daFrameDelta* dest;// = daKeyFrameDeltaGetDrawaltarget->getDeltaByDrawableID(d.getDrawableID());
                    daFrameDelta* moveBy;// = daKeyFrameAnimationComputeMotionDelta(nextDelta, dest, intermediates, i);
                    daFrameDeltaPrintDebug(moveBy);
                    daFrameAddMotionDelta(frame,moveBy);
                }
		daKeyFrameAnimationAddFrame(kfa,frame);
                kfa->currentFrame++;
            }
            // Move on to the next KeyFrame
            kfa->currentKeyFrame++;
        }
    }

void daKeyFrameAnimationAddFrame(daKeyFrameAnimation* keyFrameAnim, daFrame* f) {
    int index = daKeyFrameAnimationGetNextAvailableFrameIndex(keyFrameAnim);
    keyFrameAnim->frames[index] = f;
}

void daKeyFrameAnimationAddKeyFrame(daKeyFrameAnimation *kfa, daKeyFrame *kf) {
    int index = daKeyFrameAnimationGetNextAvailableKeyFrameIndex(kfa);
    kfa->keyFrames[index] = kf;
    return;
}

int daKeyFrameAnimationGetNextAvailableKeyFrameIndex(daKeyFrameAnimation* kfa) {
	int retval = -1;
	int i;
	daKeyFrame* next = NULL;
	for(i=0;i<DA_KEYFRAMEANIM_FRAMES_SZ;i++){
		next=kfa->keyFrames[i];
		if(next == NULL) {
			retval = i;
			break;
		}
	}
	return retval;
}

int daKeyFrameAnimationGetNextAvailableFrameIndex(daKeyFrameAnimation* kfa) {
	int retval = -1;
	int i;
	daFrame* next = NULL;
	for(i=0;i<DA_KEYFRAMEANIM_FRAMES_SZ;i++){
		next=kfa->frames[i];
		if(next == NULL) {
			retval = i;
			break;
		}
	}
	return retval;
}

int daKeyFrameAnimationGetNextAvailableDrawableIndex(daKeyFrameAnimation* kfa) {
	int retval = -1;
	int i;
	int next = -1;
	for(i=0;i<DA_KEYFRAMEANIM_FRAMES_SZ;i++){
		next=kfa->drawables[i];
		if(next == -1) {
			retval = i;
			break;
		}
	}
	return retval;
}
void daKeyFrameAnimationAddDrawable(daKeyFrameAnimation* kfa, int sd) {
    int index =  daKeyFrameAnimationGetNextAvailableDrawableIndex(kfa);
    kfa->drawables[index] = sd;
    return;
}

void daKeyFrameAnimationRemoveDrawable(daKeyFrameAnimation *kfa, int dID) {
    kfa->drawables[dID] = -1;
    return;
}

void daKeyFrameAnimationNextFrame(daKeyFrameAnimation* kfa) {
    // We're done
    if (kfa->done) {
        return;
    }

    fprintf(stdout,"Applying next Frame: %d\n" , kfa->currentFrame);
    daFrame *currentFrame = kfa->frames[kfa->currentFrame];

    if (currentFrame == NULL) {
        if (kfa->keyFrames[kfa->numKeyFrames]->wrap) {
            kfa->currentFrame = 0;
        } else {
            kfa->done = 1;
            return;
        }
        //return;
    }

    int i;
    daFrameDelta *nextFrameDelta;
    for (i=0; i<DA_FRAME_DELTA_SZ; i++) {  //FrameDelta d : currentFrame.getMotionDeltas())
	nextFrameDelta = currentFrame->frameDeltas[i];
        if(nextFrameDelta == NULL) {
            continue;
        }

        //SceneObject sd = getDrawableByID(d.getDrawableID());
        daKeyFrameAnimationApplyFrameDeltaToVector(nextFrameDelta,NULL,NULL);
    }

    kfa->currentFrame++;
}

void daKeyFrameAnimationApplyFrameDeltaToVector(
		daFrameDelta* delta, 
		float* posVector, 
		float* rotVector) { 
	posVector[DA_X] += delta->positionDelta[DA_X];
	posVector[DA_Y] += delta->positionDelta[DA_Y];
	posVector[DA_Z] += delta->positionDelta[DA_Z];

	rotVector[DA_X] += delta->rotationDelta[DA_X];
	rotVector[DA_Y] += delta->rotationDelta[DA_Y];
	rotVector[DA_Z] += delta->rotationDelta[DA_Z];
}
