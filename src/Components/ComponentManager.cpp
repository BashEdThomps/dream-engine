#include "ComponentManager.h"

namespace Dream {
  namespace Components {
    ComponentManager::ComponentManager(Time* time, Video::Camera* camera) {
      mTime = time;
      mCamera = camera;
      mParallel           = false;
      mAnimationComponent = NULL;
      mAudioComponent     = NULL;
      mInputComponent     = NULL;
      mScriptingComponent = NULL;
      mPhysicsComponent   = NULL;
      mVideoComponent     = NULL;
      mDone               = false;
    }

    ComponentManager::~ComponentManager() {
      if (mParallel) {
        joinThreads();
      }
      destroyComponents();
    }

    void ComponentManager::destroyComponents() {
      delete mAnimationComponent;
      delete mAudioComponent;
      delete mInputComponent;
      delete mScriptingComponent;
      delete mPhysicsComponent;
      delete mVideoComponent;
    }

    bool ComponentManager::createComponents() {
      std::cout << "ComponentManager: Creating Components..." << std::endl;
      if(!createScriptingComponent()) return false;
      if(!createAudioComponent())     return false;
      if(!createPhysicsComponent())   return false;
      if(!createVideoComponent())     return false;
      if(!createInputComponent())     return false;
      if(!createAnimationComponent()) return false;
      std::cout << "ComponentManager: Successfuly created Components." << std::endl;
      return true;
    }

    bool ComponentManager::createScriptingComponent() {
      mScriptingComponent = new Scripting::ChaiScripting();
      if (!mScriptingComponent->init()) {
        std::cerr << "ComponentManager: Unable to initialise ChaiScripting." << std::endl;
        return false;
      }
      return mScriptingComponent != NULL;
    }

    bool ComponentManager::createAudioComponent() {
      mAudioComponent = new Audio::OpenALAudio();
      if (!mAudioComponent->init()) {
        std::cerr << "ComponentManager: Unable to initialise OpenALAudio." << std::endl;
        return false;
      }
      return mAudioComponent != NULL;
    }

    bool ComponentManager::createPhysicsComponent() {
      mPhysicsComponent = new Physics::BulletPhysics();
      if (!mPhysicsComponent->init()){
        std::cerr << "ComponentManager: Unable to initialise BulletPhysics." << std::endl;
        return false;
      }
      return mPhysicsComponent != NULL;
    }

    bool ComponentManager::createVideoComponent() {
      mVideoComponent = new Video::OpenGLVideo(mCamera);
      mVideoComponent->setScreenName("Dream::OpenGL");
      if (mVideoComponent->init()) {
        return true;
      } else {
        std::cerr << "ComponentManager: Unable to initialise OpenGLVideo." << std::endl;
        return false;
      }
      return false;
    }

    bool ComponentManager::createAnimationComponent() {
      mAnimationComponent = new Animation::DreamAnimation(mTime);
      if (mAnimationComponent->init()) {
        return true;
      } else {
        std::cerr << "ComponentManager: Unable to initialise DreamAnimation." << std::endl;
        return false;
      }
      return true;
    }

    bool ComponentManager::createInputComponent() {
      mInputComponent = new Input::GLFWInput();
      try {
        Video::OpenGLVideo* ogl   = dynamic_cast<Video::OpenGLVideo*>(mVideoComponent);
        Input::GLFWInput*   input = dynamic_cast<Input::GLFWInput*>(mInputComponent);
        input->setWindow(ogl->getWindow());
      } catch (std::exception ex) {
        std::cerr << "ComponentManager: " << ex.what() << std::endl;
        return false;
      }

      if (!mInputComponent->init()) {
        std::cerr << "ComponentManager: Unable to initialise GLFWInputComponent." << std::endl;
        return false;
      }
      return mInputComponent != NULL;
    }

    void ComponentManager::startThreads() {

      if (mAnimationComponent != NULL) {
        mAnimationComponentThread = new std::thread(&ComponentManager::componentThread, this,mAnimationComponent);
      }

      if (mPhysicsComponent != NULL) {
        mPhysicsComponentThread = new std::thread(&ComponentManager::componentThread, this,mPhysicsComponent);
      }

      if (mAudioComponent != NULL) {
        mAudioComponentThread = new std::thread(&ComponentManager::componentThread, this,mAudioComponent);
      }
    }

    void ComponentManager::componentThread(ComponentInterface* component) {
      component->init();
      while (!mDone) {
        component->update(mActiveScene);
        std::this_thread::yield();
      }
    }

    void ComponentManager::joinThreads() {
      if (mAnimationComponentThread != NULL && mAnimationComponentThread->joinable()) {
        mAnimationComponentThread->join();
        delete mAnimationComponentThread;
        mAnimationComponentThread = NULL;
      }

      if (mAudioComponentThread != NULL && mAudioComponentThread->joinable()) {
        mAudioComponentThread->join();
        delete mAudioComponentThread;
        mAudioComponentThread = NULL;
      }

      if (mPhysicsComponentThread != NULL && mPhysicsComponentThread->joinable()) {
        mPhysicsComponentThread->join();
        delete mPhysicsComponentThread;
        mPhysicsComponentThread = NULL;
      }
    }

    bool ComponentManager::isDone() {
      return mDone;
    }

    void ComponentManager::update() {
      mDone = mVideoComponent->isWindowShouldCloseFlagSet();
      if(!mParallel) {
        mAudioComponent->update(mActiveScene);
        mPhysicsComponent->update(mActiveScene);
        mAnimationComponent->update(mActiveScene);
      }
      mInputComponent->update(mActiveScene);
      mScriptingComponent->update(mActiveScene);
      mVideoComponent->update(mActiveScene);
    }

    void ComponentManager::setActiveScene(Scene* scene) {
      mActiveScene = scene;
    }

    void ComponentManager::populatePhysicsWorld(std::vector<SceneObject*> soWithPhysicsObjects) {
      std::cout << "ComponentManager: Populating Physics World" << std::endl;
      try {
        std::vector<SceneObject*>::iterator soIter;
        Physics::BulletPhysics* bulletPhysicsComponent;
        bulletPhysicsComponent = dynamic_cast<Physics::BulletPhysics*>(mPhysicsComponent);
        for (soIter = soWithPhysicsObjects.begin(); soIter != soWithPhysicsObjects.end(); soIter++) {
          #ifdef VERBOSE
          std::cout << "ComponentManager: Adding SceneObject " << (*soIter)->getUUID()
                    << " to PhysicsComponent World" << std::endl;
          #endif
          Physics::PhysicsObjectInstance* physicsObject;
          physicsObject = dynamic_cast<Physics::PhysicsObjectInstance*>(
            (*soIter)->getPhysicsObjectAssetInstance()
          );
          bulletPhysicsComponent->addPhysicsObjectInstance(physicsObject);
        }
      } catch (const std::exception &ex) {
        std::cerr << "ComponentManager: Exception thrown while populating physics world..."
                  << std::endl << ex.what() << std::endl;;
      }
    }

    Animation::AnimationComponentInterface* ComponentManager::getAnimationComponent() {
      return mAnimationComponent;
    }

    Audio::AudioComponentInterface* ComponentManager::getAudioComponent() {
      return mAudioComponent;
    }

    Input::InputComponentInterface* ComponentManager::getInputComponent() {
      return mInputComponent;
    }

    Physics::PhysicsComponentInterface* ComponentManager::getPhysicsComponent() {
      return mPhysicsComponent;
    }

    Video::VideoComponentInterface* ComponentManager::getVideoComponent() {
      return mVideoComponent;
    }

    bool ComponentManager::isParallel() {
      return mParallel;
    }

    void ComponentManager::setParallel(bool parallel) {
      mParallel = parallel;
    }

  } // End of Components
} // End of Dream
