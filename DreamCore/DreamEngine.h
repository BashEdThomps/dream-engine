#ifndef DREAMENGINE_H
#define DREAMENGINE_H

#include <iostream>
#include <thread>
#include <dirent.h>
#include <memory>

#include "Constants.h"
#include "ArgumentParser.h"
#include "Time.h"
#include "FileReader.h"
#include "Scene.h"
#include "Project.h"
#include "IAssetInstance.h"

#include "GameController.h"

#include "Components/Animation/AnimationComponent.h"
#include "Components/Audio/AudioComponent.h"
#include "Components/Graphics/Camera.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Components/Physics/PhysicsComponent.h"
#include "Components/Window/IWindowComponent.h"

#include "Lua/LuaEngine.h"

using namespace std;

namespace Dream
{
    class DreamEngine
    {
    private:
        bool    mDone;
        Time    mTime;
        Project mProject;
        Scene   mActiveScene;
        Camera  mCamera;
        GameController mGameController;

        // Components
        bool mComponentsInitialised;
        shared_ptr<AudioComponent>     mAudioComponent;
        shared_ptr<GraphicsComponent>  mGraphicsComponent;
        shared_ptr<PhysicsComponent>   mPhysicsComponent;
        shared_ptr<AnimationComponent> mAnimationComponent;
        shared_ptr<IWindowComponent>   mWindowComponent;
        shared_ptr<LuaEngine>          mLuaEngine;

    public:
        DreamEngine(shared_ptr<IWindowComponent> windowComponent);
        ~DreamEngine();

        bool heartbeat();

        bool loadFromArgumentParser(ArgumentParser);
        bool loadFromDirectory(string);
        bool initEngine();
        bool initComponents();
        bool initLuaEngine();

        bool loadProjectFromFileReader(string projectPath, FileReader&);
        bool loadScene(Scene);
        bool loadSceneByUuid(string);
        Project getProject();
        void setProject(Project&);
        bool isProjectLoaded();

        bool updateLogic();
        bool updateGraphics();
        bool updateCleanup();

        void setTime(Time&);
        Time getTime();
        void setDone(bool);
        bool isDone();

        Scene getActiveScene();

        Camera getCamera();
        void setCamera(Camera&);

        bool initAnimationComponent();
        bool initAudioComponent();
        bool initPhysicsComponent();
        bool initGraphicsComponent();
        bool initWindowComponent();

        void setComponentsInitialised(bool initd);
        bool getComponentsInitialised();

        void stopActiveScene();
        void cleanupComponents();

        shared_ptr<AnimationComponent> getAnimationComponent();
        void setAnimationComponent(shared_ptr<AnimationComponent>);

        shared_ptr<AudioComponent> getAudioComponent();
        void setAudioComponent(shared_ptr<AudioComponent>);

        shared_ptr<PhysicsComponent> getPhysicsComponent();
        void setPhysicsComponent(shared_ptr<PhysicsComponent>);

        shared_ptr<GraphicsComponent> getGraphicsComponent();
        void setGraphicsComponent(shared_ptr<GraphicsComponent>);

        shared_ptr<IWindowComponent> getWindowComponent();
        void setWindowComponent(shared_ptr<IWindowComponent>);

        GameController getGameController();

        void setGameController(GameController&);
        void setActiveScene(Scene&);

    }; // End of DreamEngine


} // End of Dream

#endif // DREAMENGINE_H
