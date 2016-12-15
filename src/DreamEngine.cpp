
#include "DreamEngine.h"

namespace Dream {

  DreamEngine* DreamEngine::_Instance = nullptr;

  DreamEngine* DreamEngine::getInstance() {
    if (_Instance == nullptr) {
      _Instance = new DreamEngine();
    }
    return _Instance;
  }

  DreamEngine::DreamEngine() {
    setAssetManager(nullptr);
    setAnimationComponent(nullptr);
    setAudioComponent(nullptr);
    setPhysicsComponent(nullptr);
    setGraphicsComponent(nullptr);
    setCamera(nullptr);
    setTime(nullptr);
    setDone(false);
    mActiveScene = nullptr;
    setProject(nullptr);
  }

  DreamEngine::~DreamEngine() {
    destroyComponents();
  }

  Project* DreamEngine::getProject() {
    return mProject;
  }

  void DreamEngine::setCamera(Camera* camera) {
    mCamera = camera;
  }

  void DreamEngine::setAudioComponent(AudioComponent* audioComp) {
    mAudioComponent = audioComp;
  }

  void DreamEngine::setAnimationComponent(AnimationComponent* animComp) {
    mAnimationComponent = animComp;
  }

  void DreamEngine::setPhysicsComponent(PhysicsComponent* physicsComp) {
    mPhysicsComponent = physicsComp;
  }

  void DreamEngine::setGraphicsComponent(GraphicsComponent* graphicsComp) {
    mGraphicsComponent = graphicsComp;
  }

  void DreamEngine::setDone(bool done) {
    mDone = done;
  }

  void DreamEngine::setTime(Time* time) {
    mTime = time;
  }

  void DreamEngine::setProject(Project* project) {
    mProject = project;
  }

  bool DreamEngine::isProjectLoaded() {
    return mProject != nullptr;
  }

  bool DreamEngine::loadProjectFromFileReader(string projectPath, FileReader* reader) {
    if (DEBUG) {
      cout << "Dream: Loading project from FileReader " << reader->getPath() << endl;
    }
    setAssetManager(new AssetManager());
    string projectJsonStr = reader->getContentsAsString();
    if (projectJsonStr.empty()) {
      cerr << "Dream: Loading Failed. Project Content is Empty" << endl;
      return false;
    }
    json projectJson = json::parse(projectJsonStr);
    if (DEBUG) {
      cout << "DreamEngine: Read Project..." << endl
           << projectJson.dump(2) << endl;
    }
    setProject(new Project(mAssetManager, projectPath, projectJson));
    mAssetManager->setProjectPath(projectPath);
    return isProjectLoaded();
  }

  bool DreamEngine::loadFromArgumentParser(ArgumentParser *parser) {
    if (DEBUG) {
      cout << "Dream: Loading from ArgumentParser" << endl;
    }
    FileReader *projectFileReader = new FileReader(parser->getProjectFilePath());
    projectFileReader->readIntoStringStream();
    bool loadSuccess = loadProjectFromFileReader(parser->getProjectPath(), projectFileReader);
    delete projectFileReader;
    delete parser;
    return loadSuccess;
  }

  void DreamEngine::destroyScene(Scene* scene) {
    mAssetManager->cleanupScene(scene);
    delete mActiveScene;
    mActiveScene = nullptr;
  }

  bool DreamEngine::loadSceneByUuid(string uuid) {
    Scene* scene = mProject->getSceneByUuid(uuid);
    return loadScene(scene);
  }

  bool DreamEngine::loadScene(Scene* scene) {
    // Check valid
    if (scene == nullptr) {
      cerr << "Dream: Cannot load scene, null!" << endl;
      return false;
    }
    // Clean up old scene
    if (mActiveScene != nullptr) {
      destroyScene(mActiveScene);
    }
    // Load the new scene

    if (DEBUG) {
      cout << "Dream: Loading Scene " << scene->getName() << endl;
    }
    mActiveScene = scene;
    mGraphicsComponent->setClearColour(mActiveScene->getClearColour());
    mCamera->setTranslation(mActiveScene->getDefaultCameraTranslation());
    mCamera->setRotation(mActiveScene->getDefaultCameraRotation());
    mCamera->setMovementSpeed(mActiveScene->getCameraMovementSpeed());
    return true;
  }

  Scene* DreamEngine::getActiveScene() {
    return mActiveScene;
  }

  AssetManager* DreamEngine::getAssetManager() {
    return mAssetManager;
  }

  bool DreamEngine::initAssetManager() {
    if (mAssetManager != nullptr) {

      if (DEBUG) {
        cout << "Dream: Destroying existing Asset Manager." << endl;
      }
      delete mAssetManager;
    }
    mAssetManager = new AssetManager();
    return mAssetManager != nullptr;
  }

  bool DreamEngine::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
      cerr << "SDL_Init Error: " << SDL_GetError() << endl;
      return false;
    }
    return true;
  }

  bool DreamEngine::initEngine() {
    // Init SDL
    if (!initSDL()) {
      cerr << "Dream:Error:Unable to initialise SDL" << endl;
      return false;
    }

    // Init Components
    if(!initComponents()){
      cerr << "Dream:Error:Unable to create components." << endl;
      return false;
    }

    // Init Startup Scene
    if (!loadScene(mProject->getStartupScene())) {
      cerr << "Dream:Error:Unable to load startup scene." << endl;
      return false;
    }

    if (DEBUG) {
      cout << "Dream:Info:Starting Startup Scene " << getActiveScene()->getNameAndUuidString() << endl;
    }
    return true;
  }

  bool DreamEngine::update() {
    mTime->update();
    mAssetManager->createAllAssetInstances(mActiveScene);
    mActiveScene->update();
    mAssetManager->removeFromLuaScriptMap(mActiveScene->getDeleteQueue());
    mActiveScene->destroyDeleteQueue();
    updateComponents();
    mDone = mDone || mGraphicsComponent->isWindowShouldCloseFlagSet();
    this_thread::yield();
    return !mDone;
  }

  void DreamEngine::setAssetManager(AssetManager* assetManager) {
    mAssetManager = assetManager;
  }

  Time* DreamEngine::getTime() {
    return mTime;
  }

  void DreamEngine::destroyComponents() {
    if (mAnimationComponent != nullptr) {
      delete mAnimationComponent;
    }

    if (mAudioComponent != nullptr) {
      delete mAudioComponent;
    }

    if (mPhysicsComponent != nullptr) {
      delete mPhysicsComponent;
    }

    if (mGraphicsComponent != nullptr) {
      delete mGraphicsComponent;
    }
  }

  bool DreamEngine::initComponents() {
    if (DEBUG) {
      cout << "Dream: Creating Components..." << endl;
    }
    setTime(new Time());
    if(!initAudioComponent()) return false;
    if(!initPhysicsComponent()) return false;
    if(!initGraphicsComponent()) return false;
    if(!initAnimationComponent()) return false;
    if (DEBUG) {
      cout << "Dream: Successfuly created Components." << endl;
    }
    return true;
  }

  bool DreamEngine::initAudioComponent() {
    mAudioComponent = new AudioComponent();
    if (!mAudioComponent->init()) {
      cerr << "Dream: Unable to initialise OpenALAudio." << endl;
      return false;
    }
    return mAudioComponent != nullptr;
  }

  bool DreamEngine::initPhysicsComponent() {
    mPhysicsComponent = new PhysicsComponent();
    mPhysicsComponent->setTime(mTime);
    if (!mPhysicsComponent->init()){
      cerr << "ComponentManager: Unable to initialise BulletPhysics." << endl;
      return false;
    }
    return mPhysicsComponent != nullptr;
  }

  bool DreamEngine::initGraphicsComponent() {
    setCamera(new Camera());
    mGraphicsComponent = new GraphicsComponent(mCamera);
    mGraphicsComponent->setWindowWidth(mProject->getWindowWidth());
    mGraphicsComponent->setWindowHeight(mProject->getWindowHeight());
    mGraphicsComponent->setScreenName(mProject->getName());
    mGraphicsComponent->setTime(mTime);

    if (mGraphicsComponent->init()) {
      return true;
    } else {
      cerr << "DreamEngine:Error:Unable to initialise Graphics Component." << endl;
      return false;
    }
  }

  bool DreamEngine::initAnimationComponent() {
    mAnimationComponent = new AnimationComponent();
    mAnimationComponent->setTime(mTime);
    if (mAnimationComponent->init()) {
      return true;
    } else {
      cerr << "DreamEngine:Error:Unable to initialise Animation Component." << endl;
      return false;
    }
  }

  bool DreamEngine::isDone() {
    return mDone;
  }

  void DreamEngine::updateComponents() {
    mDone = mGraphicsComponent->isWindowShouldCloseFlagSet();
    mGraphicsComponent->update(mActiveScene);
    mEvent = mGraphicsComponent->getSDL_Event();
  }

  AnimationComponent* DreamEngine::getAnimationComponent() {
    return mAnimationComponent;
  }

  AudioComponent* DreamEngine::getAudioComponent() {
    return mAudioComponent;
  }

  PhysicsComponent* DreamEngine::getPhysicsComponent() {
    return mPhysicsComponent;
  }

  GraphicsComponent* DreamEngine::getGraphicsComponent() {
    return mGraphicsComponent;
  }

  Camera* DreamEngine::getCamera() {
    return mCamera;
  }

  map<SceneObject*,LuaScriptInstance*>* DreamEngine::getLuaScriptMap() {
    return mAssetManager->getLuaScriptMap();
  }

  SDL_Event DreamEngine::getSDL_Event() {
    return mEvent;
  }
} // End of Dream
