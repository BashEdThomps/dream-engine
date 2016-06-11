#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <vector>

#include "../../../AssetInstance.h"
#include "../../../AssetDefinition.h"
#include "../../../../Scene/SceneObject.h"

#include "KeyFrame.h"
#include "Frame.h"

#define ASSET_DIR_ANIMATION    "animation"
#define ASSET_FORMAT_ANIMATION "animation"

#define DREAM_ANIMATION_STOPPED 0
#define DREAM_ANIMATION_PLAYING 1
#define DREAM_ANIMATION_PAUSED  2


namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Animation {
namespace Dream {
	
  class AnimationInstance : public AssetInstance {
	private:
		static int FramesPerSecond;
	public:
  	static void setFramesPerSecond(int);
    static int  getFramesPerSecond();
  private:
    std::string            mAnimationPath;
    std::vector<KeyFrame*> mKeyFrames;
    std::vector<Frame*>    mPlaybackFrames;
		Scene::SceneObject*    mSceneObject;
		int                    mCurrentPlaybackFrame;
		bool                   mLoop;
		bool                   mFinished;
	public:
		AnimationInstance(AssetDefinition*);
    ~AnimationInstance();
		bool load(std::string);
	
		void setSceneObject(Scene::SceneObject*);
		Scene::SceneObject* getSceneObject();
		
		bool isLooping();
		int  getState();
		void play();
		void stop();
		void pause();
		
	private:
		void generatePlaybackFrames();
    void addKeyFrame(KeyFrame*);
    void addFrame(Frame*);
  }; // End of AnimationInstance
	  
} // End of Dream
} // End of Animation
} // End of Instances
} // End of Asset
} // End of Dream

#endif // ANIMAITON_H
