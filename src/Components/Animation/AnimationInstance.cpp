
#include "AnimationInstance.h"
#include <algorithm>
#include "../../String.h"

namespace Dream {

      int AnimationInstance::FramesPerSecond = 60;

      void AnimationInstance::setFramesPerSecond(int fps) {
        FramesPerSecond = fps;
      }

      int AnimationInstance::getFramesPerSecond() {
        return FramesPerSecond;
      }

      AnimationInstance::AnimationInstance(
          AssetDefinition* definition,
          Transform3D* transform) : AssetInstance(definition,transform) {
        mCurrentPlaybackFrame = 0;
        mLoop                 = false;
        mPlaying              = false;
      }

      AnimationInstance::~AnimationInstance() {}

      void AnimationInstance::showStatus() {
        cout << "AnimationInstance:" << endl;
        cout << "\tLoop: " << String::boolToYesNo(mLoop) << endl;
      }

      bool AnimationInstance::load(string projectPath) {
        mLoop = mDefinition->getJson()[ASSET_ATTR_LOOP];
        loadExtraAttributes(mDefinition->getJson());
        return false;
      }

      void AnimationInstance::addKeyFrame(KeyFrame *kf) {
        mKeyFrames.push_back(kf);
      }

      void AnimationInstance::generatePlaybackFrames() {
        cout<<"AnimationInstance: Generating Playback Frames" << endl;
        vector<KeyFrame*>::iterator keyFrameIter;
        for (keyFrameIter = mKeyFrames.begin(); keyFrameIter != mKeyFrames.end(); keyFrameIter++) {
          KeyFrame* currentKeyFrame = (*keyFrameIter);
          // Get the next KeyFrame
          KeyFrame* nextKeyFrame = nullptr;
          // End of Vector?
          if (currentKeyFrame == mKeyFrames.back()) {
            cout << "AnimationInstance: Last KeyFrame, checking for wrap..." << endl;
            if (currentKeyFrame->getWrap()) {
              cout << "AnimationInstance: KeyFrame wraps to beginning." << endl;
              nextKeyFrame = (*mKeyFrames.begin());
            } else {
              cout << "AnimationInstance: Last KeyFrame does not wrap." << endl;
              break;
            }
          } else {
            nextKeyFrame = *(keyFrameIter+1);
          }

          currentKeyFrame->generatePlaybackFrames(nextKeyFrame);
          vector<Frame*> frames = currentKeyFrame->getPlaybackFrames();
          mPlaybackFrames.insert(mPlaybackFrames.end(),frames.begin(),frames.end());
        }
        cout<<"AnimationInstance: Finished Generating Playback Frames" << endl;
      }

      void AnimationInstance::loadExtraAttributes(nlohmann::json jsonObj) {
        showStatus();
        if (!jsonObj[ASSET_ATTR_KEYFRAMES].is_null() && jsonObj[ASSET_ATTR_KEYFRAMES].is_array()){
          nlohmann::json jsonKeyFrames = jsonObj[ASSET_ATTR_KEYFRAMES];
          cout << "AnimationInstance: Loading KeyFrames" << endl;
          for (nlohmann::json::iterator it = jsonKeyFrames.begin(); it != jsonKeyFrames.end(); ++it) {
            KeyFrame *nextKeyFrame = new KeyFrame();
            vector<float> translation(3), rotation(3), scale(3);

            translation[0] = (*it)[ASSET_ATTR_TRANSLATION][ASSET_ATTR_X];
            translation[1] = (*it)[ASSET_ATTR_TRANSLATION][ASSET_ATTR_Y];
            translation[2] = (*it)[ASSET_ATTR_TRANSLATION][ASSET_ATTR_Z];

            rotation[0] = (*it)[ASSET_ATTR_ROTATION][ASSET_ATTR_X];
            rotation[1] = (*it)[ASSET_ATTR_ROTATION][ASSET_ATTR_Y];
            rotation[2] = (*it)[ASSET_ATTR_ROTATION][ASSET_ATTR_Z];

            scale[0] = (*it)[ASSET_ATTR_SCALE][ASSET_ATTR_X];
            scale[1] = (*it)[ASSET_ATTR_SCALE][ASSET_ATTR_Y];
            scale[2] = (*it)[ASSET_ATTR_SCALE][ASSET_ATTR_Z];

            long startTime   = (*it)[ASSET_ATTR_START_TIME];

            bool wrap = false;
            if (!(*it)[ASSET_ATTR_WRAP].is_null()){
              wrap = (*it)[ASSET_ATTR_WRAP];
            }

            string interpolation = (*it)[ASSET_ATTR_INTERPOLATION];
            string name = (*it)[ASSET_NAME];
            string uuid = (*it)[ASSET_UUID];

            nextKeyFrame->setName(name);
            nextKeyFrame->setInterpolationType(interpolation);
            nextKeyFrame->setUUID(uuid);
            nextKeyFrame->setStartTimeMS(startTime);
            nextKeyFrame->setTranslation(translation);
            nextKeyFrame->setRotation(rotation);
            nextKeyFrame->setScale(scale);
            nextKeyFrame->setWrap(wrap);

            nextKeyFrame->showStatus();

            addKeyFrame(nextKeyFrame);
          }
          generatePlaybackFrames();
        }
      }

      void AnimationInstance::step(double deltaTime) {
        if (mPlaying) {
          int advanceBy = ceil(deltaTime / (1000/getFramesPerSecond()));
          if (advanceBy > MAX_FRAME_ADVANCE) return;
          mCurrentPlaybackFrame += advanceBy;
          //cout << "AnimationInstance: Delta time: " << deltaTime << ", Advance By: " << advanceBy <<  " frames to frame: " << mCurrentPlaybackFrame << endl;
          if (mCurrentPlaybackFrame > mPlaybackFrames.size()) {
            if (!mLoop) {
              cout << "AnimationInstance: Playback Finished" << endl;
              mPlaying = false;
            }
            cout << "AnimationInstance: Returning to Frame 0" << endl;
            mCurrentPlaybackFrame = 0;
          }
        }
      }

      void AnimationInstance::play() {
        cout << "AnimationInstance: Playing Animation" << getName() << endl;
        mPlaying = true;
      }

      void AnimationInstance::pause() {
        cout << "AnimationInstance: Pausing Animation" << getName() << endl;
        mPlaying = false;
      }

      void AnimationInstance::stop() {
        cout << "AnimationInstance: Stopping Animation" << getName() << endl;
        mPlaying = false;
        mCurrentPlaybackFrame = 0;
      }

      bool AnimationInstance::isLooping() {
        return mLoop;
      }

      void AnimationInstance::setLooping(bool looping) {
        mLoop = looping;
      }

      void AnimationInstance::applyTransform(Transform3D* transform) {
        if (mPlaying && mCurrentPlaybackFrame < mPlaybackFrames.size()) {
          Frame *currentFrame = mPlaybackFrames[mCurrentPlaybackFrame];
          currentFrame->applyToTransform(transform);
        }
      }

} // End of Dream
