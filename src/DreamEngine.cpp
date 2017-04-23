
#include "DreamEngine.h"
#include "JoystickManager.h"
#include "GameController.h"

namespace Dream {

  DreamEngine* DreamEngine::_Instance = nullptr;

  DreamEngine* DreamEngine::getInstance() {
    if (_Instance == nullptr) {
      _Instance = new DreamEngine();
    }
    return _Instance;
  }

  DreamEngine::DreamEngine() {
    setAnimationComponent(nullptr);
    setAudioComponent(nullptr);
    setPhysicsComponent(nullptr);
    setGraphicsComponent(nullptr);
    setCamera(nullptr);
    setTime(nullptr);
    setDone(false);
    mActiveScene = nullptr;
    setProject(nullptr);
    mGameController = new GameController();
  }

  DreamEngine::~DreamEngine() {
     if (DEBUG) {
         cout << "DreamEngine: Destroying Object" << endl;
     }
    destroyComponents();
    JoystickManager::destroy();
    if (mGameController) {
        delete mGameController;
    }
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
    string projectJsonStr = reader->getContentsAsString();
    if (projectJsonStr.empty()) {
      cerr << "Dream: Loading Failed. Project Content is Empty" << endl;
      return false;
    }
    json projectJson = json::parse(projectJsonStr);
    if (VERBOSE) {
      cout << "DreamEngine: Read Project..." << endl
           << projectJson.dump(2) << endl;
    }
    setProject(new Project(projectPath, projectJson));
    return isProjectLoaded();
  }

  bool DreamEngine::loadFromArgumentParser(ArgumentParser *parser) {
    if (VERBOSE) {
      cout << "Dream: Loading from ArgumentParser" << endl;
    }
    FileReader *projectFileReader = new FileReader(parser->getProjectFilePath());
    projectFileReader->readIntoStringStream();
    bool loadSuccess = loadProjectFromFileReader(parser->getProjectPath(), projectFileReader);
    delete projectFileReader;
    delete parser;
    return loadSuccess;
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
    // Load the new scene
    if (DEBUG) {
      cout << "Dream: Loading Scene " << scene->getName() << endl;
    }
    mActiveScene = scene;
    mGraphicsComponent->setClearColour(mActiveScene->getClearColour());
    mGraphicsComponent->setAmbientLightColour(mActiveScene->getAmbientLightColour());
    mPhysicsComponent->setGravity(mActiveScene->getGravity());
    mPhysicsComponent->setDebug(mActiveScene->getPhysicsDebug());
    mCamera->setTranslation(mActiveScene->getDefaultCameraTranslation());
    mCamera->setRotation(mActiveScene->getDefaultCameraRotation());
    mCamera->setMovementSpeed(mActiveScene->getCameraMovementSpeed());
    return true;
  }

  Scene* DreamEngine::getActiveScene() {
    return mActiveScene;
  }

  bool DreamEngine::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER ) != 0){
      cerr << "SDL_Init Error: " << SDL_GetError() << endl;
      return false;
    }
    JoystickManager::init();
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
    if (VERBOSE) {
      cout << "==== DreamEngine: Update Called ====" << endl;
    }
    // Update Time
    mTime->update();
    // Create new Assets
    mActiveScene->createAllAssetInstances();
    // Update Components
    mGraphicsComponent->update(mActiveScene);
    // Draw 3D
    mGraphicsComponent->draw3DQueue();
    mPhysicsComponent->setViewProjectionMatrix(
      mGraphicsComponent->getViewMatrix(),
      mGraphicsComponent->getProjectionMatrix()
    );
    mPhysicsComponent->update(mActiveScene);
    mPhysicsComponent->drawDebug();
    // Draw 2D
    mGraphicsComponent->draw2DQueue();
    mGraphicsComponent->swapBuffers();
    // Update state
    mEvents = mGraphicsComponent->getSDL_Events();
    mDone = mGraphicsComponent->isWindowShouldCloseFlagSet();
    // Cleanup Old
    mActiveScene->findDeletedSceneObjects();
    mActiveScene->findDeletedScripts();
    mActiveScene->destroyDeleteQueue();
    mActiveScene->clearDeleteQueue();
    mActiveScene->generateScenegraphVector();
    // Chill
    this_thread::yield();
    return !mDone;
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
    if(!initGraphicsComponent()) return false;
    if(!initPhysicsComponent()) return false;
    if(!initAudioComponent()) return false;
    if(!initAnimationComponent()) return false;
    if (DEBUG) {
      cout << "Dream: Successfuly created Components." << endl;
    }
    return true;
  }

  bool DreamEngine::initAudioComponent() {
    mAudioComponent = new AudioComponent();
    if (!mAudioComponent->init()) {
      cerr << "Dream: Unable to initialise AudioComponent." << endl;
      return false;
    }
    return mAudioComponent != nullptr;
  }

  bool DreamEngine::initPhysicsComponent() {
    mPhysicsComponent = new PhysicsComponent();
    mPhysicsComponent->setTime(mTime);
    if (!mPhysicsComponent->init()){
      cerr << "ComponentManager: Unable to initialise PhysicsComponent." << endl;
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
        mGraphicsComponent->setGameController(mGameController);
      return true;
    } else {
      cerr << "DreamEngine: Unable to initialise Graphics Component." << endl;
      return false;
    }
  }

  bool DreamEngine::initAnimationComponent() {
    mAnimationComponent = new AnimationComponent();
    mAnimationComponent->setTime(mTime);
    if (mAnimationComponent->init()) {
      return true;
    } else {
      cerr << "DreamEngine: Unable to initialise Animation Component." << endl;
      return false;
    }
  }

  bool DreamEngine::isDone() {
    return mDone;
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
    return mActiveScene->getLuaScriptMap();
  }

  vector<SDL_Event> DreamEngine::getSDL_Events() {
    return mEvents;
  }

  GameController* DreamEngine::getGameController() {
      return mGameController;
  }

} // End of Dream
