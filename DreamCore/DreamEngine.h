#ifndef DREAMENGINE_H
#define DREAMENGINE_H

#include <iostream>
#include <thread>

#include "Constants.h"
#include "ArgumentParser.h"
#include "Time.h"
#include "FileReader.h"
#include "Scene.h"
#include "Project.h"
#include "IAssetInstance.h"

#include "GameController.h"

#include "Components/Animation/AnimationComponent.h"
#include "Components/Audio/IAudioComponent.h"
#include "Components/Graphics/Camera.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Components/Physics/PhysicsComponent.h"
#include "Components/Window/IWindowComponent.h"

#include "Lua/ILuaExposable.h"

namespace Dream
{
    template <class AudioComponent, class WindowComponent>
    class DreamEngine : public ILuaExposable
    {
    private:
        Time *mTime;
        Project *mProject;
        Scene *mActiveScene;
        Camera *mCamera;
        IAudioComponent *mAudioComponent;
        GraphicsComponent *mGraphicsComponent;
        PhysicsComponent *mPhysicsComponent;
        AnimationComponent *mAnimationComponent;
        IWindowComponent *mWindowComponent;
        bool mDone;
        GameController *mGameController;
    public:
        DreamEngine(void);
        ~DreamEngine(void);
        bool loadFromArgumentParser(ArgumentParser*);
        bool initEngine();
        bool initSDL();
        bool initAssetManager();
        bool initComponents();

        bool loadProjectFromFileReader(string projectPath, FileReader*);
        bool loadScene(Scene*);
        bool loadSceneByUuid(string);
        Project* getProject();
        void setProject(Project*);
        bool isProjectLoaded();

        bool update();
        void setTime(Time*);
        Time* getTime();
        void setDone(bool);
        bool isDone();

        Scene *getActiveScene();

        Camera* getCamera();
        void setCamera(Camera*);

        void destroyComponents();
        bool initAnimationComponent();
        bool initAudioComponent();
        bool initPhysicsComponent();
        bool initGraphicsComponent();
        bool initWindowComponent();

        void destroyScene(Scene*);

        AnimationComponent* getAnimationComponent();
        void setAnimationComponent(AnimationComponent*);

        IAudioComponent* getAudioComponent();
        void setAudioComponent(IAudioComponent*);

        PhysicsComponent* getPhysicsComponent();
        void setPhysicsComponent(PhysicsComponent*);

        GraphicsComponent* getGraphicsComponent();
        void setGraphicsComponent(GraphicsComponent*);

        map<SceneObject*,LuaScriptInstance*>* getLuaScriptMap();
        GameController* getGameController();

        void setGameController(GameController*);
        void setActiveScene(Scene*);

        void exposeLuaApi(lua_State*);

    }; // End of DreamEngine

    template <class AudioComponent, class WindowComponent >
    DreamEngine<AudioComponent,WindowComponent>::
    DreamEngine() : ILuaExposable()
    {
        dreamSetDebug(true);
        dreamSetVerbose(true);
        setAnimationComponent(nullptr);
        setAudioComponent(nullptr);
        setPhysicsComponent(nullptr);
        setGraphicsComponent(nullptr);
        setCamera(nullptr);
        setTime(nullptr);
        setDone(false);
        setActiveScene(nullptr);
        setProject(nullptr);
        setGameController(new GameController());
    }

    template <class AudioComponent, class WindowComponent >
    DreamEngine<AudioComponent,WindowComponent>::
    ~DreamEngine()
    {
        if (DEBUG)
        {
            cout << "DreamEngine: Destroying Object" << endl;
        }
        destroyComponents();
        if (mGameController)
        {
            delete mGameController;
        }
    }

    template <class AudioComponent, class WindowComponent >
    Project* DreamEngine<AudioComponent,WindowComponent>::
    getProject()
    {
        return mProject;
    }

    template <class AudioComponent, class WindowComponent >
    void DreamEngine<AudioComponent,WindowComponent>::setCamera(Camera* camera)
    {
        mCamera = camera;
    }

    template <class AudioComponent, class WindowComponent >
    void DreamEngine<AudioComponent,WindowComponent>::
    setAudioComponent(IAudioComponent* audioComp)
    {
        mAudioComponent = audioComp;
    }

    template <class AudioComponent, class WindowComponent >
    void DreamEngine<AudioComponent,WindowComponent>::
    setAnimationComponent(AnimationComponent* animComp)
    {
        mAnimationComponent = animComp;
    }

    template <class AudioComponent, class WindowComponent >
    void DreamEngine<AudioComponent,WindowComponent>::
    setPhysicsComponent(PhysicsComponent* physicsComp)
    {
        mPhysicsComponent = physicsComp;
    }

    template <class AudioComponent, class WindowComponent >
    void DreamEngine<AudioComponent,WindowComponent>::
    setGraphicsComponent(GraphicsComponent* graphicsComp)
    {
        mGraphicsComponent = graphicsComp;
    }

    template <class AudioComponent, class WindowComponent >
    void DreamEngine<AudioComponent,WindowComponent>::
    setDone(bool done)
    {
        mDone = done;
    }

    template <class AudioComponent, class WindowComponent >
    void DreamEngine<AudioComponent,WindowComponent>::
    setTime(Time* time)
    {
        mTime = time;
    }

    template <class AudioComponent, class WindowComponent >
    void DreamEngine<AudioComponent,WindowComponent>::
    setProject(Project* project)
    {
        mProject = project;
    }

    template <class AudioComponent, class WindowComponent >
    bool DreamEngine<AudioComponent,WindowComponent>::
    isProjectLoaded()
    {
        return mProject != nullptr;
    }

    template <class AudioComponent, class WindowComponent >
    bool DreamEngine<AudioComponent,WindowComponent>::
    loadProjectFromFileReader(string projectPath, FileReader* reader)
    {
        if (DEBUG)
        {
            cout << "Dream: Loading project from FileReader " << reader->getPath() << endl;
        }
        string projectJsonStr = reader->getContentsAsString();
        if (projectJsonStr.empty())
        {
            cerr << "Dream: Loading Failed. Project Content is Empty" << endl;
            return false;
        }
        json projectJson = json::parse(projectJsonStr);
        if (VERBOSE)
        {
            cout << "DreamEngine: Read Project..." << endl
                 << projectJson.dump(2) << endl;
        }
        setProject(new Project(projectPath, projectJson, mAudioComponent));
        return isProjectLoaded();
    }

    template <class AudioComponent, class WindowComponent >
    bool DreamEngine<AudioComponent,WindowComponent>::
    loadFromArgumentParser(ArgumentParser *parser)
    {
        if (VERBOSE)
        {
            cout << "Dream: Loading from ArgumentParser" << endl;
        }
        FileReader *projectFileReader = new FileReader(parser->getProjectFilePath());
        projectFileReader->readIntoStringStream();
        bool loadSuccess = loadProjectFromFileReader(parser->getProjectPath(), projectFileReader);
        delete projectFileReader;
        delete parser;
        return loadSuccess;
    }

    template <class AudioComponent, class WindowComponent >
    bool DreamEngine<AudioComponent,WindowComponent>::
    loadSceneByUuid(string uuid)
    {
        Scene* scene = mProject->getSceneByUuid(uuid);
        return loadScene(scene);
    }

    template <class AudioComponent, class WindowComponent >
    bool DreamEngine<AudioComponent,WindowComponent>::
    loadScene(Scene* scene)
    {
        // Check valid
        if (scene == nullptr)
        {
            cerr << "Dream: Cannot load scene, null!" << endl;
            return false;
        }
        // Load the new scene
        if (DEBUG)
        {
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

    template <class AudioComponent, class WindowComponent >
    Scene* DreamEngine<AudioComponent,WindowComponent>::
    getActiveScene()
    {
        return mActiveScene;
    }


    template <class AudioComponent, class WindowComponent >
    bool DreamEngine<AudioComponent,WindowComponent>::
    initEngine()
    {
        // Init Components
        if(!initComponents())
        {
            cerr << "Dream:Error:Unable to create components." << endl;
            return false;
        }

        // Init Startup Scene
        if (!loadScene(mProject->getStartupScene()))
        {
            cerr << "Dream:Error:Unable to load startup scene." << endl;
            return false;
        }

        if (DEBUG)
        {
            cout << "Dream:Info:Starting Startup Scene " << getActiveScene()->getNameAndUuidString() << endl;
        }
        return true;
    }

    template <class AudioComponent, class WindowComponent >
    bool DreamEngine<AudioComponent,WindowComponent>::
    update()
    {
        if (VERBOSE)
        {
            cout << "==== DreamEngine: Update Called ====" << endl;
        }
        // Update Time
        mTime->update();
        // Update Window
        mWindowComponent->update(mActiveScene);
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
        mWindowComponent->swapBuffers();
        // Update state
        mDone = mWindowComponent->shouldClose();
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


    template <class AudioComponent, class WindowComponent >
    Time* DreamEngine<AudioComponent,WindowComponent>::
    getTime()
    {
        return mTime;
    }

    template <class AudioComponent, class WindowComponent >
    void DreamEngine<AudioComponent,WindowComponent>::
    destroyComponents()
    {
        if (mAnimationComponent != nullptr)
        {
            delete mAnimationComponent;
            mAnimationComponent = nullptr;
        }

        if (mAudioComponent != nullptr)
        {
            delete mAudioComponent;
            mAudioComponent = nullptr;
        }

        if (mPhysicsComponent != nullptr)
        {
            delete mPhysicsComponent;
            mPhysicsComponent = nullptr;
        }

        if (mGraphicsComponent != nullptr)
        {
            delete mGraphicsComponent;
            mGraphicsComponent = nullptr;
        }
        if (mWindowComponent != nullptr)
        {
            delete mWindowComponent;
            mWindowComponent = nullptr;
        }
    }

    template <class AudioComponent, class WindowComponent >
    bool DreamEngine<AudioComponent,WindowComponent>::
    initComponents()
    {
        if (DEBUG)
        {
            cout << "Dream: Creating Components..." << endl;
        }

        setTime(new Time());

        if (!initWindowComponent())
        {
            return false;
        }

        if(!initGraphicsComponent())
        {
            return false;
        }

        if(!initPhysicsComponent())
        {
            return false;
        }

        if(!initAudioComponent())
        {
            return false;
        }

        if(!initAnimationComponent())
        {
            return false;
        }

        if (DEBUG)
        {
            cout << "Dream: Successfuly created Components." << endl;
        }

        return true;
    }

    template <class AudioComponent, class WindowComponent >
    bool DreamEngine<AudioComponent,WindowComponent>::
    initWindowComponent()
    {
       mWindowComponent = new WindowComponent();

       mWindowComponent->setWidth(mProject->getWindowWidth());
       mWindowComponent->setHeight(mProject->getWindowHeight());
       mWindowComponent->setName(mProject->getName());

       if (!mWindowComponent->init())
       {
           cerr << "DreamEngine: Unable to initialise WindowComponent" << endl;
           return false;
       }

       return mWindowComponent != nullptr;
    }

    template <class AudioComponent, class WindowComponent >
    bool DreamEngine<AudioComponent,WindowComponent>::
    initAudioComponent()
    {
        mAudioComponent = new AudioComponent();
        if (!mAudioComponent->init())
        {
            cerr << "Dream: Unable to initialise AudioComponent." << endl;
            return false;
        }
        return mAudioComponent != nullptr;
    }

    template <class AudioComponent, class WindowComponent >
    bool DreamEngine<AudioComponent,WindowComponent>::
    initPhysicsComponent()
    {
        mPhysicsComponent = new PhysicsComponent();
        mPhysicsComponent->setTime(mTime);
        if (!mPhysicsComponent->init())
        {
            cerr << "ComponentManager: Unable to initialise PhysicsComponent." << endl;
            return false;
        }
        return mPhysicsComponent != nullptr;
    }

    template <class AudioComponent, class WindowComponent >
    bool DreamEngine<AudioComponent,WindowComponent>::
    initGraphicsComponent()
    {
        setCamera(new Camera());

        mGraphicsComponent = new GraphicsComponent(mCamera,mWindowComponent);
        mGraphicsComponent->setTime(mTime);

        if (mGraphicsComponent->init())
        {
            mGraphicsComponent->setGameController(mGameController);
            return true;
        }
        else
        {
            cerr << "DreamEngine: Unable to initialise Graphics Component." << endl;
            return false;
        }
    }

    template <class AudioComponent, class WindowComponent >
    bool DreamEngine<AudioComponent,WindowComponent>::
    initAnimationComponent()
    {
        mAnimationComponent = new AnimationComponent();
        mAnimationComponent->setTime(mTime);
        if (mAnimationComponent->init())
        {
            return true;
        }
        else
        {
            cerr << "DreamEngine: Unable to initialise Animation Component." << endl;
            return false;
        }
    }

    template <class AudioComponent, class WindowComponent >
    bool DreamEngine<AudioComponent,WindowComponent>::
    isDone()
    {
        return mDone;
    }

    template <class AudioComponent, class WindowComponent >
    AnimationComponent* DreamEngine<AudioComponent,WindowComponent>::
    getAnimationComponent()
    {
        return mAnimationComponent;
    }

    template <class AudioComponent, class WindowComponent >
    IAudioComponent* DreamEngine<AudioComponent,WindowComponent>::
    getAudioComponent()
    {
        return mAudioComponent;
    }

    template <class AudioComponent, class WindowComponent >
    PhysicsComponent* DreamEngine<AudioComponent,WindowComponent>::
    getPhysicsComponent()
    {
        return mPhysicsComponent;
    }

    template <class AudioComponent, class WindowComponent >
    GraphicsComponent* DreamEngine<AudioComponent,WindowComponent>::
    getGraphicsComponent()
    {
        return mGraphicsComponent;
    }

    template <class AudioComponent, class WindowComponent >
    Camera* DreamEngine<AudioComponent,WindowComponent>::
    getCamera()
    {
        return mCamera;
    }

    template <class AudioComponent, class WindowComponent >
    map<SceneObject*,LuaScriptInstance*>* DreamEngine<AudioComponent,WindowComponent>::
    getLuaScriptMap()
    {
        return mActiveScene->getLuaScriptMap();
    }

    template <class AudioComponent, class WindowComponent >
    GameController* DreamEngine<AudioComponent,WindowComponent>::
    getGameController()
    {
        return mGameController;
    }

    template <class AudioComponent, class WindowComponent >
    void DreamEngine<AudioComponent,WindowComponent>::
    setGameController(GameController *gc)
    {
        mGameController = gc;
    }

    template <class AudioComponent, class WindowComponent >
    void DreamEngine<AudioComponent,WindowComponent>::
    setActiveScene(Scene *scene)
    {
        mActiveScene = scene;
    }

    template <class AudioComponent, class WindowComponent >
    void DreamEngine<AudioComponent,WindowComponent>::
    exposeLuaApi(lua_State* state)
    {
        luabind::module(state) [
                luabind::class_<DreamEngine>("DreamEngine")
                .def("getActiveScene",&DreamEngine::getActiveScene)
                .def("getTime",&DreamEngine::getTime)
                .def("loadSceneByUuid",&DreamEngine::loadSceneByUuid)
                .def("getGraphicsComponent",&DreamEngine::getGraphicsComponent)
                .def("getPhysicsComponent",&DreamEngine::getPhysicsComponent)
                .def("getCamera",&DreamEngine::getCamera)
                .def("getGameController",&DreamEngine::getGameController)
                .scope
                [
                luabind::def("setDebug",&dreamSetDebug),
                luabind::def("setVerbose",&dreamSetVerbose)
                ]
                ];
    }

} // End of Dream

#endif // DREAMENGINE_H
