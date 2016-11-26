#ifndef DREAM_H
#define DREAM_H

#include <iostream>
#include <thread>

#include <SDL2/SDL.h>

#include "json.hpp"

#include "ArgumentParser.h"
#include "Time.h"
#include "FileReader.h"
#include "Scene.h"
#include "Project.h"
#include "AssetInstance.h"
#include "AssetManager.h"

#include "Components/Graphics/Camera.h"
#include "Components/Animation/AnimationComponent.h"
#include "Components/Audio/AudioComponent.h"
#include "Components/Physics/PhysicsComponent.h"
#include "Components/Graphics/GraphicsComponent.h"

namespace Dream {
  class Dream {
  private:
    Time *mTime;
    Project *mProject;
    Scene* mActiveScene;
    AssetManager *mAssetManager;
    Components::Graphics::Camera *mCamera;
    Components::Audio::AudioComponent *mAudioComponent;
    Components::Graphics::GraphicsComponent *mGraphicsComponent;
    Components::Physics::PhysicsComponent *mPhysicsComponent;
    Components::Animation::AnimationComponent *mAnimationComponent;
    bool mDone;
  public:
    Dream(void);
    ~Dream(void);
    bool initSDL();
    bool loadFromArgumentParser(ArgumentParser*);
    bool loadProjectFromFileReader(std::string projectPath, FileReader*);
    bool loadScene(Scene*);
    Project* getProject();
    void setProject(Project*);
    bool isProjectLoaded();

    bool createAssetManager();
    AssetManager* getAssetManager();

    bool run();
    void setTime(Time*);
    Time* getTime();
    void setDone(bool);
    bool isDone();

    void   setActiveScene(Scene*);
    Scene *getActiveScene();
    bool   hasActiveScene();
    bool   initActiveScene();

    void setAssetManager(AssetManager*);
    Components::Graphics::Camera* getCamera();
    void setCamera(Components::Graphics::Camera*);

    void updateComponents();
    bool createComponents();
    void destroyComponents();
    bool createAnimationComponent();
    bool createAudioComponent();
    bool createPhysicsComponent();
    bool createGraphicsComponent();

    Components::Animation::AnimationComponent* getAnimationComponent();
    void setAnimationComponent(Components::Animation::AnimationComponent*);

    Components::Audio::AudioComponent* getAudioComponent();
    void setAudioComponent(Components::Audio::AudioComponent*);

    Components::Physics::PhysicsComponent* getPhysicsComponent();
    void setPhysicsComponent(Components::Physics::PhysicsComponent*);

    Components::Graphics::GraphicsComponent* getGraphicsComponent();
    void setGraphicsComponent(Components::Graphics::GraphicsComponent*);

  }; // End of Dream
} // End of Dream

#endif // DREAM_H
