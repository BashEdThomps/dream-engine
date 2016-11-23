
#include "Dream.h"

namespace Dream {

  Dream::Dream() {
    setAssetManager(NULL);
    mAnimationComponent = NULL;
    mAudioComponent     = NULL;
    mInputComponent     = NULL;
    mPhysicsComponent   = NULL;
    mVideoComponent     = NULL;

    setDone(false);
    setCamera(new Components::Video::Camera());
    setTime(new Time());
    setActiveScene(NULL);
    createComponents();
    createAssetManager();
    setProject(NULL);
  }

  Dream::~Dream() {
      destroyComponents();
  }

  Project* Dream::getProject() {
    return mProject;
  }

  void Dream::setCamera(Components::Video::Camera* camera) {
    mCamera = camera;
  }

  void Dream::setDone(bool done) {
    mDone = done;
  }

  void Dream::setTime(Time* time) {
    mTime = time;
  }

  void Dream::setProject(Project* project) {
    mProject = project;
  }

  bool Dream::isProjectLoaded() {
    return mProject != NULL;
  }

  bool Dream::loadProjectFromFileReader(std::string projectPath, FileReader* reader)
  {
    std::cout << "Dream: Loading project from FileReader " << reader->getPath() << std::endl;
    std::string projectJsonStr = reader->getContentsAsString();

    //std::cout << "Dream: Read Project:" << std::endl << projectJsonStr << std::endl;

    if (projectJsonStr.empty())
    {
      std::cerr << "Dream: Loading Failed. Project Content is Empty" << std::endl;
      return false;
    }

    nlohmann::json projectJson = nlohmann::json::parse(projectJsonStr);

    setProject(new Project(mAssetManager, projectPath, projectJson));
    mAssetManager->setProjectPath(projectPath);

    return isProjectLoaded();
  }

  bool Dream::loadFromArgumentParser(ArgumentParser *parser)
  {
    std::cout << "Dream: Loading from ArgumentParser" << std::endl;
    FileReader *projectFileReader = new FileReader(parser->getProjectFilePath());
    projectFileReader->readIntoStringStream();
    bool loadSuccess = loadProjectFromFileReader(parser->getProjectPath(), projectFileReader);
    delete projectFileReader;
    delete parser;
    return loadSuccess;
  }

  bool Dream::loadScene(Scene* scene)
  {
    std::cout << "Dream: Loading Scene " << scene->getName() << std::endl;
    setActiveScene(scene);

    if (!hasActiveScene()) {
      std::cerr << "Dream: Unable to find active scene. Cannot Continue." << std::endl;
      return false;
    }

    if (!initActiveScene()) {
      std::cerr << "Dream: Unable to initialise Current Scene." << std::endl;
      return false;
    }
    return true;
  }

  void Dream::setActiveScene(Scene* scene)
  {
    mActiveScene = scene;
  }

  Scene* Dream::getActiveScene()
  {
    return mActiveScene;
  }

  bool Dream::hasActiveScene() {
    return getActiveScene() != NULL;
  }

  bool Dream::initActiveScene()
  {
    if (mActiveScene != NULL)
    {
      if (!mActiveScene->init())
      {
        return false;
      }
    }

    if (!mAssetManager->createAllAssetInstances(mActiveScene))
    {
      std::cerr << "Dream: Unable to create asset instances." << std::endl;
      return false;
    }

    mPhysicsComponent->populatePhysicsWorld(
        mAssetManager->getSceneObjectsWithPhysicsObjects()
    );

    mCamera->setTranslation(mActiveScene->getDefaultCameraTranslation());
    mCamera->setRotation(mActiveScene->getDefaultCameraRotation());
    mCamera->setMovementSpeed(mActiveScene->getCameraMovementSpeed());

    return true;
  }

  AssetManager* Dream::getAssetManager() {
    return mAssetManager;
  }

  bool Dream::createAssetManager() {
    if (mAssetManager != NULL) {
      std::cout << "Dream: Destroying existing Asset Manager." << std::endl;
      delete mAssetManager;
    }
    mAssetManager = new AssetManager();
    return mAssetManager != NULL;
  }


  bool Dream::run() {
    // Create Components
    if(!createComponents()){
      std::cerr << "Dream: Unable to create components." << std::endl;
      return false;
    }

    // Init Scene Asset Instances
    if (!loadScene(mProject->getStartupScene())) {
      std::cerr << "Dream: Unable to load startup scene." << std::endl;
      return false;
    }

    std::cout << "Dream: Starting Scene - "
              << getActiveScene()->getName()
              << " (" << getActiveScene()->getUUID() << ")" << std::endl;

    // GameLoop

    while(!mDone) {
      mTime->update();
      updateComponents();
      std::this_thread::yield();
    }

    return mDone;
  }

  void Dream::setAssetManager(AssetManager* assetManager) {
    mAssetManager = assetManager;
  }

  Time* Dream::getTime() {
    return mTime;
  }

  void Dream::destroyComponents() {
      delete mAnimationComponent;
      delete mAudioComponent;
      delete mInputComponent;
      delete mPhysicsComponent;
      delete mVideoComponent;
    }

    bool Dream::createComponents() {
      std::cout << "Dream: Creating Components..." << std::endl;
      if(!createAudioComponent())     return false;
      if(!createPhysicsComponent())   return false;
      if(!createVideoComponent())     return false;
      if(!createInputComponent())     return false;
      if(!createAnimationComponent()) return false;
      std::cout << "Dream: Successfuly created Components." << std::endl;
      return true;
    }

    bool Dream::createAudioComponent() {
      mAudioComponent = new Components::Audio::AudioComponent();
      if (!mAudioComponent->init()) {
        std::cerr << "Dream: Unable to initialise OpenALAudio." << std::endl;
        return false;
      }
      return mAudioComponent != NULL;
    }

    bool Dream::createPhysicsComponent() {
      mPhysicsComponent = new Components::Physics::PhysicsComponent();
      mPhysicsComponent->setTime(mTime);
      if (!mPhysicsComponent->init()){
        std::cerr << "ComponentManager: Unable to initialise BulletPhysics." << std::endl;
        return false;
      }
      return mPhysicsComponent != NULL;
    }

    bool Dream::createVideoComponent() {
      mVideoComponent = new Components::Video::VideoComponent(mCamera);
      mVideoComponent->setScreenName("Dream::OpenGL");
      mVideoComponent->setTime(mTime);
      if (mVideoComponent->init()) {
        return true;
      } else {
        std::cerr << "ComponentManager: Unable to initialise OpenGLVideo." << std::endl;
        return false;
      }
    }

    bool Dream::createAnimationComponent() {
      mAnimationComponent = new Components::Animation::AnimationComponent(mTime);
      mAnimationComponent->setTime(mTime);
      if (mAnimationComponent->init()) {
        return true;
      } else {
        std::cerr << "ComponentManager: Unable to initialise DreamAnimation." << std::endl;
        return false;
      }
    }

    bool Dream::createInputComponent() {
      mInputComponent = new Components::Input::InputComponent();
      mInputComponent->setTime(mTime);
      try {
        Components::Video::VideoComponent* ogl   = dynamic_cast<Components::Video::VideoComponent*>(mVideoComponent);
        Components::Input::InputComponent* input = dynamic_cast<Components::Input::InputComponent*>(mInputComponent);
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

    bool Dream::isDone() {
      return mDone;
    }

    void Dream::updateComponents() {
      mDone = mVideoComponent->isWindowShouldCloseFlagSet();
      mInputComponent->update(mActiveScene);
      mVideoComponent->update(mActiveScene);
    }

    Components::Animation::AnimationComponent* Dream::getAnimationComponent() {
      return mAnimationComponent;
    }

    Components::Audio::AudioComponent* Dream::getAudioComponent() {
      return mAudioComponent;
    }

    Components::Input::InputComponent* Dream::getInputComponent() {
      return mInputComponent;
    }

    Components::Physics::PhysicsComponent* Dream::getPhysicsComponent() {
      return mPhysicsComponent;
    }

    Components::Video::VideoComponent* Dream::getVideoComponent() {
      return mVideoComponent;
    }

    Components::Video::Camera* Dream::getCamera() {
      return mCamera;
    }


} // End of Dream
