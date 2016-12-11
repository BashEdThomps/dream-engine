
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
    setActiveScene(nullptr);
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
    cout << "Dream: Loading project from FileReader " << reader->getPath() << endl;
    setAssetManager(new AssetManager());
    string projectJsonStr = reader->getContentsAsString();
    if (projectJsonStr.empty()) {
      cerr << "Dream: Loading Failed. Project Content is Empty" << endl;
      return false;
    }
    json projectJson = json::parse(projectJsonStr);
    setProject(new Project(mAssetManager, projectPath, projectJson));
    mAssetManager->setProjectPath(projectPath);
    return isProjectLoaded();
  }

  bool DreamEngine::loadFromArgumentParser(ArgumentParser *parser) {
    cout << "Dream: Loading from ArgumentParser" << endl;
    FileReader *projectFileReader = new FileReader(parser->getProjectFilePath());
    projectFileReader->readIntoStringStream();
    bool loadSuccess = loadProjectFromFileReader(parser->getProjectPath(), projectFileReader);
    delete projectFileReader;
    delete parser;
    return loadSuccess;
  }

  bool DreamEngine::loadScene(Scene* scene) {
    if (scene == nullptr) {
      cerr << "Dream: Cannot load scene, null!" << endl;
      return false;
    }

    cout << "Dream: Loading Scene " << scene->getName() << endl;
    setActiveScene(scene);

    if (!hasActiveScene()) {
      cerr << "Dream: Unable to find active scene. Cannot Continue." << endl;
      return false;
    }

    if (!initActiveScene()) {
      cerr << "Dream: Unable to initialise Current Scene." << endl;
      return false;
    }
    return true;
  }

  void DreamEngine::setActiveScene(Scene* scene) {
    mActiveScene = scene;
  }

  Scene* DreamEngine::getActiveScene() {
    return mActiveScene;
  }

  bool DreamEngine::hasActiveScene() {
    return getActiveScene() != nullptr;
  }

  bool DreamEngine::initActiveScene() {
    if (mActiveScene != nullptr) {
      if (!mActiveScene->init()) {
        return false;
      }
    }

    mGraphicsComponent->setClearColour(mActiveScene->getClearColour());

    if (!mAssetManager->createAllAssetInstances(mActiveScene)) {
      cerr << "Dream: Unable to create asset instances." << endl;
      return false;
    }

    mPhysicsComponent->populatePhysicsWorld(mAssetManager->getSceneObjectsWithPhysicsObjects());

    mCamera->setTranslation(mActiveScene->getDefaultCameraTranslation());
    mCamera->setRotation(mActiveScene->getDefaultCameraRotation());
    mCamera->setMovementSpeed(mActiveScene->getCameraMovementSpeed());

    return true;
  }

  AssetManager* DreamEngine::getAssetManager() {
    return mAssetManager;
  }

  bool DreamEngine::createAssetManager() {
    if (mAssetManager != nullptr) {
      cout << "Dream: Destroying existing Asset Manager." << endl;
      delete mAssetManager;
    }
    mAssetManager = new AssetManager();
    return mAssetManager != nullptr;
  }

  bool DreamEngine::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
      cout << "SDL_Init Error: " << SDL_GetError() << endl;
      return false;
    }
    return true;
  }

  bool DreamEngine::bootstrap() {

    if (!initSDL()) {
      cerr << "Dream: Unable to initialise SDL" << endl;
      return false;
    }

    // Create Components
    if(!createComponents()){
      cerr << "Dream: Unable to create components." << endl;
      return false;
    }

    // Init Scene Asset Instances
    if (!loadScene(mProject->getStartupScene())) {
      cerr << "Dream: Unable to load startup scene." << endl;
      return false;
    }

    cout << "Dream: Starting Scene - "
         << getActiveScene()->getName()
         << " (" << getActiveScene()->getUUID() << ")" << endl;
    return true;
  }

  bool DreamEngine::update() {
    mTime->update();
    updateComponents();
    mActiveScene->destroyObjectDeleteQueue();
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

  bool DreamEngine::createComponents() {
    cout << "Dream: Creating Components..." << endl;
    setTime(new Time());
    if(!createAudioComponent()) return false;
    if(!createPhysicsComponent()) return false;
    if(!createGraphicsComponent()) return false;
    if(!createAnimationComponent()) return false;
    cout << "Dream: Successfuly created Components." << endl;
    return true;
  }

  bool DreamEngine::createAudioComponent() {
    mAudioComponent = new AudioComponent();
    if (!mAudioComponent->init()) {
      cerr << "Dream: Unable to initialise OpenALAudio." << endl;
      return false;
    }
    return mAudioComponent != nullptr;
  }

  bool DreamEngine::createPhysicsComponent() {
    mPhysicsComponent = new PhysicsComponent();
    mPhysicsComponent->setTime(mTime);
    if (!mPhysicsComponent->init()){
      cerr << "ComponentManager: Unable to initialise BulletPhysics." << endl;
      return false;
    }
    return mPhysicsComponent != nullptr;
  }

  bool DreamEngine::createGraphicsComponent() {
    setCamera(new Camera());
    mGraphicsComponent = new GraphicsComponent(mCamera);
    mGraphicsComponent->setWindowWidth(mProject->getWindowWidth());
    mGraphicsComponent->setWindowHeight(mProject->getWindowHeight());
    mGraphicsComponent->setScreenName(mProject->getName());
    mGraphicsComponent->setTime(mTime);

    if (mGraphicsComponent->init()) {
      return true;
    } else {
      cerr << "ComponentManager: Unable to initialise OpenGLGraphics." << endl;
      return false;
    }
  }

  bool DreamEngine::createAnimationComponent() {
    mAnimationComponent = new AnimationComponent();
    mAnimationComponent->setTime(mTime);
    if (mAnimationComponent->init()) {
      return true;
    } else {
      cerr << "ComponentManager: Unable to initialise DreamAnimation." << endl;
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
