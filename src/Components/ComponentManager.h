#ifndef ComponentManager_hpp
#define ComponentManager_hpp

#include <iostream>
#include <thread>

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
#include "Animation/DreamAnimation.h"
#include "Input/GLFWInput.h"
#include "Video/Camera.h"
#include "../Time.h"


namespace Dream {
  namespace Components {
    class ComponentManager {
    private:
      Scene *mActiveScene;
      bool   mDone;
      bool   mParallel;
      Time  *mTime;
      Video::Camera *mCamera;

      Audio::AudioComponentInterface *mAudioComponent;
      Video::VideoComponentInterface *mVideoComponent;
      Physics::PhysicsComponentInterface *mPhysicsComponent;
      Scripting::ScriptingComponentInterface *mScriptingComponent;
      Input::InputComponentInterface *mInputComponent;
      Animation::AnimationComponentInterface *mAnimationComponent;

      std::thread *mAudioComponentThread;
      std::thread *mPhysicsComponentThread;
      std::thread *mAnimationComponentThread;

    public:
      ComponentManager(Time*, Video::Camera*);
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

      Animation::AnimationComponentInterface* getAnimationComponent();
      Audio::AudioComponentInterface* getAudioComponent();
      Input::InputComponentInterface* getInputComponent();
      Physics::PhysicsComponentInterface* getPhysicsComponent();
      Video::VideoComponentInterface* getVideoComponent();

      void componentThread(ComponentInterface*);
      void update();
      void setActiveScene(Dream::Scene*);
      void setParallel(bool);
      bool isParallel();
    }; // End of ComponentManager
  } // End of Components
} // End Dream

#endif /* ComponentManager_hpp */
