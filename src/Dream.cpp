
#include "Dream.h"

namespace Dream {

    Dream::Dream() {
        setAssetManager(NULL);
        mAnimationComponent = NULL;
        mAudioComponent     = NULL;
        mPhysicsComponent   = NULL;
        mGraphicsComponent     = NULL;

        setDone(false);
        setCamera(new Components::Graphics::Camera());
        setTime(new Time());
        setActiveScene(NULL);
        createAssetManager();
        setProject(NULL);
    }

    Dream::~Dream() {
        destroyComponents();
    }

    Project* Dream::getProject() {
        return mProject;
    }

    void Dream::setCamera(Components::Graphics::Camera* camera) {
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

    bool Dream::initSDL() {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
            std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return false;
        }
        return true;
    }


    bool Dream::run() {

        if (!initSDL()) {
            std::cerr << "Dream: Unable to initialise SDL" << std::endl;
            return false;
        }

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
        delete mPhysicsComponent;
        delete mGraphicsComponent;
    }

    bool Dream::createComponents() {
        std::cout << "Dream: Creating Components..." << std::endl;
        if(!createAudioComponent())     return false;
        if(!createPhysicsComponent())   return false;
        if(!createGraphicsComponent())     return false;
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

    bool Dream::createGraphicsComponent() {
        mGraphicsComponent = new Components::Graphics::GraphicsComponent(mCamera);
        mGraphicsComponent->setScreenName(mProject->getName());
        mGraphicsComponent->setTime(mTime);
        if (mGraphicsComponent->init()) {
            return true;
        } else {
            std::cerr << "ComponentManager: Unable to initialise OpenGLGraphics." << std::endl;
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

    bool Dream::isDone() {
        return mDone;
    }

    void Dream::updateComponents() {
        mDone = mGraphicsComponent->isWindowShouldCloseFlagSet();
        mGraphicsComponent->update(mActiveScene);
    }

    Components::Animation::AnimationComponent* Dream::getAnimationComponent() {
        return mAnimationComponent;
    }

    Components::Audio::AudioComponent* Dream::getAudioComponent() {
        return mAudioComponent;
    }

    Components::Physics::PhysicsComponent* Dream::getPhysicsComponent() {
        return mPhysicsComponent;
    }

    Components::Graphics::GraphicsComponent* Dream::getGraphicsComponent() {
        return mGraphicsComponent;
    }

    Components::Graphics::Camera* Dream::getCamera() {
        return mCamera;
    }


} // End of Dream
