#include "KeyFrame.h"
#include "AnimationInstance.h"
#include "../../String.h"

namespace Dream {

  KeyFrame::KeyFrame() {
    mInterpolationType = DREAM_ANIMATION_INTERPOLATION_NONE;
  }

  KeyFrame::~KeyFrame() {}

  bool KeyFrame::getWrap() {
    return mWrap;
  }

  long KeyFrame::getStartTimeMS() {
    return mStartTimeMS;
  }

  vector<Frame*> KeyFrame::getPlaybackFrames() {
    return mPlaybackFrames;
  }

  void KeyFrame::generatePlaybackFrames(KeyFrame* toKeyFrame) {
    if (DEBUG) {
    cout << "KeyFrame: Generating " << mInterpolationType << " playback frames from "
         << mName << " to " << toKeyFrame->getName() <<  endl;
    }
    long keyFrameDurationMS = toKeyFrame->getStartTimeMS() - getStartTimeMS();
    long numFrames = static_cast<long>(
          (keyFrameDurationMS/1000.0f) * AnimationInstance::getFramesPerSecond()
          );

    if (isInterpolationTypeNone()) {
      generateNoneInterpolationFrames(toKeyFrame,numFrames);
    } else if (isInterpolationTypeLinear()) {
      generateLinearInterpolationFrames(toKeyFrame,numFrames);
    } else if (isInterpolationTypeBezier()) {
      generateBezierInterpolationFrames(toKeyFrame,numFrames);
    }
  }

  bool KeyFrame::isInterpolationTypeNone() {
    return mInterpolationType.compare(DREAM_ANIMATION_INTERPOLATION_NONE) == 0;
  }

  bool KeyFrame::isInterpolationTypeLinear() {
    return mInterpolationType.compare(DREAM_ANIMATION_INTERPOLATION_LINEAR) == 0;
  }

  bool KeyFrame::isInterpolationTypeBezier() {
    return mInterpolationType.compare(DREAM_ANIMATION_INTERPOLATION_BEZIER) == 0;
  }

  void KeyFrame::generateNoneInterpolationFrames(KeyFrame* toKeyFrame, long numFrames) {
    for (int frameIndex = 0; frameIndex < numFrames; frameIndex++) {
      Frame *nextFrame = new Frame();
      nextFrame->setTranslation(mTranslation);
      nextFrame->setRotation(mRotation);
      nextFrame->setScale(mScale);
      //nextFrame->showStatus();
      addPlaybackFrame(nextFrame);
    }
  }

  void KeyFrame::generateLinearInterpolationFrames(KeyFrame* toKeyFrame, long numFrames) {
    vector<float> toTranslation     = toKeyFrame->getTranslation();
    vector<float> toTranslationStep =  vector<float>(3);
    toTranslationStep[0] = ((toTranslation[0] - mTranslation[0]) / numFrames);
    toTranslationStep[1] = ((toTranslation[1] - mTranslation[1]) / numFrames);
    toTranslationStep[2] = ((toTranslation[2] - mTranslation[2]) / numFrames);

    vector<float> toRotation     = toKeyFrame->getRotation();
    vector<float> toRotationStep =  vector<float>(3);
    toRotationStep[0] = ((toRotation[0] - mRotation[0]) / numFrames);
    toRotationStep[1] = ((toRotation[1] - mRotation[1]) / numFrames);
    toRotationStep[2] = ((toRotation[2] - mRotation[2]) / numFrames);

    vector<float> toScale     = toKeyFrame->getScale();
    vector<float> toScaleStep =  vector<float>(3);
    toScaleStep[0] = ((toScale[0] - mScale[0]) / numFrames);
    toScaleStep[1] = ((toScale[1] - mScale[1]) / numFrames);
    toScaleStep[2] = ((toScale[2] - mScale[2]) / numFrames);

    for (int frameIndex = 0; frameIndex < numFrames; frameIndex++) {
      Frame *nextFrame = new Frame();

      vector<float> nextFrameTranslation =  vector<float>(3);
      nextFrameTranslation[0] = mTranslation[0] + (toTranslationStep[0]*frameIndex);
      nextFrameTranslation[1] = mTranslation[1] + (toTranslationStep[1]*frameIndex);
      nextFrameTranslation[2] = mTranslation[2] + (toTranslationStep[2]*frameIndex);
      nextFrame->setTranslation(nextFrameTranslation);

      vector<float> nextFrameRotation =  vector<float>(3);
      nextFrameRotation[0] = mRotation[0] + (toRotationStep[0]*frameIndex);
      nextFrameRotation[1] = mRotation[1] + (toRotationStep[1]*frameIndex);
      nextFrameRotation[2] = mRotation[2] + (toRotationStep[2]*frameIndex);
      nextFrame->setRotation(nextFrameRotation);

      vector<float> nextFrameScale =  vector<float>(3);
      nextFrameScale[0] = mScale[0] + (toScaleStep[0]*frameIndex);
      nextFrameScale[1] = mScale[1] + (toScaleStep[1]*frameIndex);
      nextFrameScale[2] = mScale[2] + (toScaleStep[2]*frameIndex);
      nextFrame->setScale(nextFrameScale);
      //nextFrame->showStatus();
      addPlaybackFrame(nextFrame);
    }
  }

  float KeyFrame::getBezierPoint(float n1, float n2, float perc) {
    float diff = n2 - n1;
    return n1 + ( diff * perc );
  }

  void KeyFrame::generateBezierInterpolationFrames(KeyFrame* toKeyFrame, long numFrames) {
    int x1 = 0, y1 = 0, z1 = 0;
    int x2 = 0, y2 = 0, z2 = 0;
    int x3 = 0, y3 = 0, z3 = 0;

    float i = 0;
    for (int frameIndex = 0; frameIndex < numFrames; frameIndex++) {
      Frame *nextFrame = new Frame();
      i = (1.0f/numFrames)*frameIndex;

      // Per-Axis Deltas
      float xa = getBezierPoint(x1, x2, i);
      float ya = getBezierPoint(y1, y2, i);
      float za = getBezierPoint(z1, z2, i);

      float xb = getBezierPoint(x2, x3, i);
      float yb = getBezierPoint(y2, y3, i);
      float zb = getBezierPoint(z2, z3, i);

      // Translation
      float x = getBezierPoint(xa, xb, i);
      float y = getBezierPoint(ya, yb, i);
      float z = getBezierPoint(za, zb, i);

      nextFrame->setTranslation({x,y,z});

      addPlaybackFrame(nextFrame);
    }
  }

  void KeyFrame::addPlaybackFrame(Frame* frame) {
    mPlaybackFrames.push_back(frame);
  }

  vector<float> KeyFrame::getTranslation() {
    return mTranslation;
  }

  vector<float> KeyFrame::getRotation() {
    return mRotation;
  }

  vector<float> KeyFrame::getScale() {
    return mScale;
  }

  void KeyFrame::setRotation( vector<float> rotation) {
    mRotation = rotation;
  }

  void KeyFrame::setScale( vector<float> scale) {
    mScale = scale;
  }

  void KeyFrame::setTranslation( vector<float> translation) {
    mTranslation = translation;
  }

  void KeyFrame::setWrap(bool wrap) {
    mWrap = wrap;
  }

  void KeyFrame::setStartTimeMS(long startTime) {
    mStartTimeMS = startTime;
  }

  string KeyFrame::getName() {
    return mName;
  }

  void KeyFrame::setName( string name) {
    mName = name;
  }

  string KeyFrame::getUUID(){
    return mUUID;
  }

  void KeyFrame::setUUID( string uuid) {
    mUUID = uuid;
  }

  void KeyFrame::showStatus() {
    if (DEBUG) {
    cout << "KeyFrame:" <<  endl
         << "\t         UUID: " << mUUID <<  endl
         << "\t         Name: " << mName <<  endl
         << "\t  Translation: " << String::floatVectorToString(mTranslation)<<  endl
         << "\t     Rotation: " << String::floatVectorToString(mRotation)<<  endl
         << "\t        Scale: " << String::floatVectorToString(mScale) <<  endl
         << "\t   Start Time: " << mStartTimeMS <<  endl
         << "\t         Wrap: " << String::boolToYesNo(mWrap) <<  endl
         << "\tInterpolation: " << getInterpolationType()
         <<  endl;
    }
  }

  void KeyFrame::setInterpolationType( string interpolationType) {
    mInterpolationType = interpolationType;
  }

  string KeyFrame::getInterpolationType() {
    return mInterpolationType;
  }

} // End of Dream
