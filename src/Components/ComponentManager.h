#ifndef ComponentManager_hpp
#define ComponentManager_hpp

#include <iostream>

#include "ComponentInterface.h"
#include "Animation/AnimationComponentInterface.h"
#include "Audio/AudioComponentInterface.h"
#include "Scripting/ScriptingComponentInterface.h"
#include "Physics/PhysicsComponentInterface.h"
#include "Input/InputComponentInterface.h"
#include "Video/VideoComponentInterface.h"
#include "Audio/OpenALAudio.h"
#include "Physics/BulletPhysics.h"
#include "Scripting/ChaiScripting.h"
#include "Video/OpenGLVideo.h"
#include "Video/VulkanVideo.h"
#include "Animation/DreamAnimation.h"
#include "Input/GLFWInput.h"

#include <thread>

namespace Dream {
  namespace Components {
    class ComponentManager {
    private:
      Project::Project *mProject;
      Scene     *mActiveScene;
      bool              mDone;
      bool              mParallel;

      Components::Audio::AudioComponentInterface *mAudioComponent;
      Components::Video::VideoComponentInterface *mVideoComponent;
      Components::Physics::PhysicsComponentInterface *mPhysicsComponent;
      Components::Scripting::ScriptingComponentInterface *mScriptingComponent;
      Components::Input::InputComponentInterface *mInputComponent;
      Components::Animation::AnimationComponentInterface *mAnimationComponent;

      std::thread *mAudioComponentThread;
      std::thread *mPhysicsComponentThread;
      std::thread *mAnimationComponentThread;

    public:
      ComponentManager(Project::Project*);
      ~ComponentManager();
      bool createComponents();
      void destroyComponents();
      bool createAnimationComponent();
      bool createAudioComponent();
      bool createInputComponent();
      bool createPhysicsComponent();
      bool createScriptingComponent();
      bool createVideoComponent();
      void startThreads();
      void joinThreads();
      bool isDone();
      void populatePhysicsWorld(std::vector<SceneObject*>);

      Components::Animation::AnimationComponentInterface* getAnimationComponent();
      Components::Audio::AudioComponentInterface* getAudioComponent();
      Components::Input::InputComponentInterface* getInputComponent();
      Components::Physics::PhysicsComponentInterface* getPhysicsComponent();
      Components::Video::VideoComponentInterface* getVideoComponent();

      void componentThread(Components::ComponentInterface*);
      void update();
      void setActiveScene(Scene*);
      void setParallel(bool);
      bool isParallel();
    }; // End of ComponentManager
  } // End of Components
} // End Dream

#endif /* ComponentManager_hpp */
