#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <vector>

#include "../../../AssetInstance.h"
#include "../../../AssetDefinition.h"
#include "../../../../Scene/SceneObject.h"

#include "KeyFrame.h"
#include "Frame.h"

#define ASSET_FORMAT_DREAM_ANIMATION "dream"

// Animation Attributes

#define ASSET_ATTR_KEYFRAMES     "keyframes"
#define ASSET_ATTR_WRAP          "wrap"
#define ASSET_ATTR_TRANSLATION   "translation"
#define ASSET_ATTR_ROTATION      "rotation"
#define ASSET_ATTR_SCALE         "scale"
#define ASSET_ATTR_START_TIME    "startTime"
#define ASSET_ATTR_WRAP          "wrap"
#define ASSET_ATTR_LOOP          "loop"
#define ASSET_ATTR_INTERPOLATION "interpolation"

#define DREAM_ANIMATION_STOPPED 0
#define DREAM_ANIMATION_PLAYING 1
#define DREAM_ANIMATION_PAUSED  2

#define MAX_FRAME_ADVANCE  10

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Animation {
namespace Dream     {
	
  class AnimationInstance : public AssetInstance {
	private:
		static int FramesPerSecond;
	public:
  	static void setFramesPerSecond(int);
    static int  getFramesPerSecond();
  private:
    std::vector<KeyFrame*> mKeyFrames;
    std::vector<Frame*>    mPlaybackFrames;
		Scene::SceneObject*    mSceneObject;
		int                    mCurrentPlaybackFrame;
		bool                   mLoop;
		bool                   mPlaying;
	public:
		AnimationInstance(AssetDefinition*);
    ~AnimationInstance();
		bool load(std::string);
	
		void setSceneObject(Scene::SceneObject*);
		Scene::SceneObject* getSceneObject();
		
		bool isLooping();
		void setLooping(bool);
		int  getState();
		void play();
		void stop();
		void pause();
		void step(double);
		void showStatus();
		
	private:
		void generatePlaybackFrames();
    void addKeyFrame(KeyFrame*);
    void addFrame(Frame*);
		void loadExtraAttributes(nlohmann::json);
  }; // End of AnimationInstance
	  
} // End of Dream
} // End of Animation
} // End of Instances
} // End of Asset
} // End of Dream

#endif // ANIMAITON_H
