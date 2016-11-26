
#include "Dream.h"

namespace Dream {

  Dream::Dream() {
      setAssetManager(nullptr);
      setAnimationComponent(nullptr);
      setAudioComponent(nullptr);
      setPhysicsComponent(nullptr);
      setGraphicsComponent(nullptr);
      setCamera(nullptr);
      setTime(nullptr);
      setDone(false);
      setActiveScene(nullptr);
      setAssetManager(nullptr);
      setProject(nullptr);
  }

  Dream::~Dream() {
      destroyComponents();
  }

  Project* Dream::getProject() {
      return mProject;
  }

  void Dream::setCamera(Camera* camera) {
      mCamera = camera;
  }

  void Dream::setAudioComponent(AudioComponent* audioComp) {
      mAudioComponent = audioComp;
  }

  void Dream::setAnimationComponent(AnimationComponent* animComp) {
      mAnimationComponent = animComp;
  }

  void Dream::setPhysicsComponent(PhysicsComponent* physicsComp) {
      mPhysicsComponent = physicsComp;
  }

  void Dream::setGraphicsComponent(GraphicsComponent* graphicsComp) {
      mGraphicsComponent = graphicsComp;
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

  bool Dream::loadProjectFromFileReader(string projectPath, FileReader* reader)
  {
      cout << "Dream: Loading project from FileReader " << reader->getPath() << endl;

      setAssetManager(new AssetManager());

      string projectJsonStr = reader->getContentsAsString();

      if (projectJsonStr.empty())
      {
          cerr << "Dream: Loading Failed. Project Content is Empty" << endl;
          return false;
      }

      json projectJson = json::parse(projectJsonStr);

      setProject(new Project(mAssetManager, projectPath, projectJson));
      mAssetManager->setProjectPath(projectPath);

      return isProjectLoaded();
  }

  bool Dream::loadFromArgumentParser(ArgumentParser *parser)
  {
      cout << "Dream: Loading from ArgumentParser" << endl;

      FileReader *projectFileReader = new FileReader(parser->getProjectFilePath());
      projectFileReader->readIntoStringStream();
      bool loadSuccess = loadProjectFromFileReader(parser->getProjectPath(), projectFileReader);

      delete projectFileReader;
      delete parser;

      return loadSuccess;
  }

  bool Dream::loadScene(Scene* scene)
  {
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
          cerr << "Dream: Unable to create asset instances." << endl;
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
          cout << "Dream: Destroying existing Asset Manager." << endl;
          delete mAssetManager;
      }
      mAssetManager = new AssetManager();
      return mAssetManager != NULL;
  }

  bool Dream::initSDL() {
      if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
          cout << "SDL_Init Error: " << SDL_GetError() << endl;
          return false;
      }
      return true;
  }


  bool Dream::run() {

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

      // GameLoop

      while(!mDone) {
          mTime->update();
          updateComponents();
          this_thread::yield();
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
      delete mPhysicsComponent;
      delete mGraphicsComponent;
  }

  bool Dream::createComponents() {
      cout << "Dream: Creating Components..." << endl;
      setTime(new Time());
      if(!createAudioComponent()) return false;
      if(!createPhysicsComponent()) return false;
      if(!createGraphicsComponent()) return false;
      if(!createAnimationComponent()) return false;
      cout << "Dream: Successfuly created Components." << endl;
      return true;
  }

  bool Dream::createAudioComponent() {
      mAudioComponent = new AudioComponent();
      if (!mAudioComponent->init()) {
          cerr << "Dream: Unable to initialise OpenALAudio." << endl;
          return false;
      }
      return mAudioComponent != NULL;
  }

  bool Dream::createPhysicsComponent() {
      mPhysicsComponent = new PhysicsComponent();
      mPhysicsComponent->setTime(mTime);
      if (!mPhysicsComponent->init()){
          cerr << "ComponentManager: Unable to initialise BulletPhysics." << endl;
          return false;
      }
      return mPhysicsComponent != NULL;
  }

  bool Dream::createGraphicsComponent() {
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

  bool Dream::createAnimationComponent() {
      mAnimationComponent = new AnimationComponent();
      mAnimationComponent->setTime(mTime);
      if (mAnimationComponent->init()) {
          return true;
      } else {
          cerr << "ComponentManager: Unable to initialise DreamAnimation." << endl;
          return false;
      }
  }

  bool Dream::isDone() {
      return mDone;
  }

  void Dream::updateComponents() {
      mDone = mGraphicsComponent->isWindowShouldCloseFlagSet();
      mGraphicsComponent->update(mActiveScene);
  }

  AnimationComponent* Dream::getAnimationComponent() {
      return mAnimationComponent;
  }

  AudioComponent* Dream::getAudioComponent() {
      return mAudioComponent;
  }

  PhysicsComponent* Dream::getPhysicsComponent() {
      return mPhysicsComponent;
  }

  GraphicsComponent* Dream::getGraphicsComponent() {
      return mGraphicsComponent;
  }

  Camera* Dream::getCamera() {
      return mCamera;
  }


} // End of Dream
