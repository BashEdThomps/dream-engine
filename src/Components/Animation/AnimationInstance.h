#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <vector>
#include "../../Constants.h"
#include "../../AssetInstance.h"
#include "../../AssetDefinition.h"
#include "KeyFrame.h"
#include "Frame.h"

namespace Dream {

  using namespace std;

  class AnimationInstance : public AssetInstance {
  private:
    static int FramesPerSecond;
  public:
    static void setFramesPerSecond(int);
    static int  getFramesPerSecond();
  private:
    vector<KeyFrame*> mKeyFrames;
    vector<Frame*> mPlaybackFrames;
    int mCurrentPlaybackFrame;
    bool mLoop;
    bool mPlaying;
  public:
    AnimationInstance(AssetDefinition*,Transform3D*);
    ~AnimationInstance();
    bool load(string);
    bool isLooping();
    void setLooping(bool);
    int  getState();
    void play();
    void stop();
    void pause();
    void step(double);
    void showStatus();
    void applyTransform(Transform3D*);
  private:
    void generatePlaybackFrames();
    void addKeyFrame(KeyFrame*);
    void addFrame(Frame*);
    void loadExtraAttributes(nlohmann::json);
    void deleteKeyFrames();
    void deletePlaybackFrames();
  }; // End of AnimationInstance

} // End of Dream

#endif // ANIMAITON_H
