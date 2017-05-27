#ifndef DREAMENGINE_H
#define DREAMENGINE_H

#include <iostream>
#include <thread>
#include <dirent.h>

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

#include "Lua/LuaEngine.h"

namespace Dream
{
    class DreamEngine
    {
    private:
        bool mDone;
        Time *mTime;
        Project *mProject;
        Scene *mActiveScene;
        Camera *mCamera;
        IAudioComponent *mAudioComponent;
        GraphicsComponent *mGraphicsComponent;
        PhysicsComponent *mPhysicsComponent;
        AnimationComponent *mAnimationComponent;
        IWindowComponent *mWindowComponent;
        GameController *mGameController;
        LuaEngine *mLuaEngine;
    public:
        DreamEngine(IAudioComponent* audioComponent, IWindowComponent* windowComponent);
        ~DreamEngine();

        bool heartbeat();

        bool loadFromArgumentParser(ArgumentParser*);
        bool loadFromDirectory(string);
        bool initEngine();
        bool initAssetManager();
        bool initComponents();
        bool initLuaEngine();

        bool loadProjectFromFileReader(string projectPath, FileReader*);
        bool loadScene(Scene*);
        bool loadSceneByUuid(string);
        Project* getProject();
        void setProject(Project*);
        bool isProjectLoaded();

        bool updateLogic();
        bool updateGraphics();
        bool updateCleanup();

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

        IWindowComponent* getWindowComponent();
        void setWindowComponent(IWindowComponent*);

        GameController* getGameController();

        void setGameController(GameController*);
        void setActiveScene(Scene*);

        void exposeLuaApi(lua_State*);

    }; // End of DreamEngine


} // End of Dream

#endif // DREAMENGINE_H
