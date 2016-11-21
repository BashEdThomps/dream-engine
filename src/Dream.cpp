
#include "Dream.h"

namespace Dream {

  Dream::Dream() {
    setDone(false);
    setTime(new Time());
    setActiveScene(NULL);
    createAssetManager();
    createComponentManager();
    mProject = NULL;
  }

  Dream::~Dream() {}

  Project* Dream::getProject() {
    return mProject;
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

  bool Dream::loadProjectFromFileReader(std::string projectPath, FileReader* reader) {
    std::cout << "Dream: Loading project from FileReader " << reader->getPath() << std::endl;
    std::string projectJsonStr = reader->getContentsAsString();
    std::cout << "Dream: Read Project:" << std::endl << projectJsonStr << std::endl;
    if (projectJsonStr.empty()) {
      std::cerr << "Dream: Loading Failed. Project Content is Empty" << std::endl;
      return false;
    }
    nlohmann::json projectJson = nlohmann::json::parse(projectJsonStr);
    setProject(new Project(mAssetManager, projectPath, projectJson));
    return isProjectLoaded();
  }

  bool Dream::loadFromArgumentParser(ArgumentParser *parser) {
    std::cout << "Dream: Loading from ArgumentParser" << std::endl;
    FileReader *projectFileReader = new FileReader(parser->getProjectFilePath());
    projectFileReader->readIntoStringStream();
    bool loadSuccess = loadProjectFromFileReader(parser->getProjectPath(), projectFileReader);
    delete projectFileReader;
    delete parser;
    return loadSuccess;
  }

  bool Dream::loadScene(Scene* scene) {
    std::cout << "Dream: Loading Scene " << scene->getName() << std::endl;
    mProject->setActiveScene(scene);

    if (!hasActiveScene()) {
      std::cerr << "Dream: Unable to find active scene. Cannot Continue." << std::endl;
      return false;
    }
    if (!initActiveScene()) {
      std::cerr << "Project: Unable to initialise Current Scene." << std::endl;
      return false;
    }
    return true;
  }

  void Dream::setActiveScene(Scene* scene) {
    mActiveScene = scene;
  }

  Scene* Dream::getActiveScene() {
    return mActiveScene;
  }

  bool Dream::hasActiveScene() {
    return getActiveScene() != NULL;
  }

  bool Dream::initActiveScene() {
    if (mActiveScene != NULL) {
      if (!mActiveScene->init()) {
        return false;
      }
    }

    if (!mAssetManager->createAllAssetInstances(mActiveScene)) {
      std::cerr << "Project: Unable to create asset instances." << std::endl;
      return false;
    }

    mComponentManager->populatePhysicsWorld(
        mAssetManager->getSceneObjectsWithPhysicsObjects()
    );

    return true;
  }

  Components::ComponentManager* Dream::getComponentManager() {
    return mComponentManager;
  }

  AssetManager* Dream::getAssetManager() {
    return mAssetManager;
  }


  bool Dream::createComponentManager() {
    if (mComponentManager != NULL) {
      std::cout << "Dream: Destroying existing ComponentManager." << std::endl;
      delete mComponentManager;
    }
    mComponentManager = new Components::ComponentManager(mTime,mCamera);
    return mComponentManager != NULL;
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
    //mComponentManager->setParallel(true);
    // Create Components
    if(!mComponentManager->createComponents()){
      std::cerr << "Dream: Unable to create components." << std::endl;
      return false;
    }

    // Init Scene with Asset Instances
    if (!loadScene(mProject->getStartupScene())) {
      std::cerr << "Dream: Unable to load startup scene." << std::endl;
      return false;
    }

    mComponentManager->setActiveScene(mActiveScene);
    std::cout << "Dream: Starting Scene - "
              << getActiveScene()->getName()
              << " (" << getActiveScene()->getUUID() << ")" << std::endl;

    // GameLoop
    if (mComponentManager->isParallel()) {
      mComponentManager->startThreads();
    }

    while(!mDone) {
      mDone = mComponentManager->isDone();
      mTime->update();
      mComponentManager->update();
      std::this_thread::yield();
    }

    if(mComponentManager->isParallel()) {
      mComponentManager->joinThreads();
    }

    return mDone;
  }

} // End of Dream
