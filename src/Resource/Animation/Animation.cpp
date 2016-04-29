#include <iostream>
#include "KeyFrame.h"
#include "FrameDelta.h"
#include "Animation.h"

namespace Dream {
	namespace Resource {
		namespace Animation {
			Animation::Animation(int fps) : Dream::Resource::Resource() {
				mFramesPerSecond = fps;
			    mCurrentFrame    = 0;
			    mCurrentKeyFrame = 0;
			}

			Animation::Animation(nlohmann::json jsonAnimation) : Dream::Resource::Resource(jsonAnimation) {

			}

			Animation::~Animation() {

			}

			void Animation::generateAbsolutePaths(std::string projectDirs, std::string projectUUID) {
				mAnimationPath = "";
			}

			void Animation::generateFrames() {
				int currentKeyFrame = 0;
				int currentFrame = 0;

				while (1) {
				    KeyFrame* source = mKeyFrames[currentKeyFrame];
				    KeyFrame* target = mKeyFrames[currentKeyFrame+1];

				    if (source == NULL) {
					std::cout << "Finished generating frames" << std::endl;
					break;
				    }

				    if (target == NULL) {
					if (source->getWrap()) {
					    target = mKeyFrames[0];
					} else {
					    std::cout << "Finished generating frames" <<  std::endl;
					    break;
					}
				    }

				    std::cout << "Generating frames for " << source->getIndex() << " >> " << target->getIndex() << std::endl;

				    int intermediates = source->getIntermediateFrameCount();
				    std::cout << "\t with " << intermediates << " intermediates" << std::endl;

				    int i;
				    for (i = 0; i < intermediates; i++) {
					Frame *frame = new Frame(currentFrame);
					int j;
					FrameDelta *nextDelta;
					int max = frame->getNumFrameDeltas();
					for (j=0;i<max;j++) { //FrameDelta d : source.getDeltas()) {
					    nextDelta = frame->getFrameDeltas()[j];
					    std::cout << "Creatng delta form " <<  nextDelta->getDrawableID() << std::endl;
					    // TODO - FIX THIS
					    FrameDelta* dest = NULL;// = KeyFrameDeltaGetDrawaltarget->getDeltaByDrawableID(d.getDrawableID());
					    FrameDelta* moveBy = NULL;// = KeyFrameAnimationComputeMotionDelta(nextDelta, dest, intermediates, i);
					    moveBy->printDebug();
					    frame->addFrameDelta(moveBy);
					}
					addFrame(frame);
					mCurrentFrame++;
				    }
				    // Move on to the next KeyFrame
				    mCurrentKeyFrame++;
				}
			}

			void Animation::addFrame(Frame* f) {
			    mFrames.push_back(f);
			}

			void Animation::addKeyFrame(KeyFrame *kf) {
			    mKeyFrames.push_back(kf);
			    return;
			}

			void Animation::addDrawable(int sd) {
			    mDrawables.push_back(sd);
			    return;
			}

			void Animation::removeDrawable(int dID) {
			    mDrawables[dID] = -1;
			    return;
			}

			void Animation::nextFrame() {
			    // We're done
			    if (mDone) {
					return;
			    }

			    std::cout << "Applying next Frame: " << mCurrentFrame << std::endl;
			    Frame *currentFrame = mFrames[mCurrentFrame];

			    if (currentFrame == NULL) {
					if (mKeyFrames[mNumKeyFrames]->getWrap()) {
					    mCurrentFrame = 0;
					} else {
					    mDone = 1;
					    return;
					}
			    }

			    int i;
			    FrameDelta *nextFrameDelta;
				int max = currentFrame->getNumFrameDeltas();
			    for (i=0; i<max; i++) {
					nextFrameDelta = currentFrame->getFrameDeltas()[i];
					if(nextFrameDelta == NULL) {
					    continue;
					}

					//SceneObject sd = getDrawableByID(d.getDrawableID());
					applyFrameDeltaToVector(nextFrameDelta,NULL,NULL);
				}

			    mCurrentFrame++;
			}

			void Animation::applyFrameDeltaToVector(
					FrameDelta* delta,
					float* posVector,
					float* rotVector) {

				posVector[0] += delta->getPositionDelta()[0];
				posVector[1] += delta->getPositionDelta()[1];
				posVector[2] += delta->getPositionDelta()[2];

				rotVector[0] += delta->getRotationDelta()[0];
				rotVector[1] += delta->getRotationDelta()[1];
				rotVector[2] += delta->getRotationDelta()[2];
			}

			int Animation::getFramesPerSecond() {
				return mFramesPerSecond;
			}
		}
	}
}
