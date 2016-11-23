#ifndef DREAM_H
#define DREAM_H

#include <iostream>
#include <thread>
#include "json.hpp"

#include "ArgumentParser.h"
#include "Time.h"
#include "FileReader.h"
#include "Scene.h"
#include "Project.h"
#include "AssetInstance.h"
#include "AssetManager.h"

#include "Components/Video/Camera.h"
#include "Components/Animation/AnimationComponent.h"
#include "Components/Audio/AudioComponent.h"
#include "Components/Input/InputComponent.h"
#include "Components/Physics/PhysicsComponent.h"
#include "Components/Video/VideoComponent.h"

namespace Dream {
  class Dream {
  private:
    Time *mTime;
    Project *mProject;
    Scene* mActiveScene;
    AssetManager *mAssetManager;
    Components::Video::Camera *mCamera;
    Components::Audio::AudioComponent *mAudioComponent;
    Components::Video::VideoComponent *mVideoComponent;
    Components::Physics::PhysicsComponent *mPhysicsComponent;
    Components::Input::InputComponent *mInputComponent;
    Components::Animation::AnimationComponent *mAnimationComponent;
    bool mDone;
  public:
    Dream(void);
    ~Dream(void);
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
    Components::Video::Camera* getCamera();
    void setCamera(Components::Video::Camera*);

    void updateComponents();
    bool createComponents();
    void destroyComponents();
    bool createAnimationComponent();
    bool createAudioComponent();
    bool createInputComponent();
    bool createPhysicsComponent();
    bool createVideoComponent();

    Components::Animation::AnimationComponent* getAnimationComponent();
    Components::Audio::AudioComponent* getAudioComponent();
    Components::Input::InputComponent* getInputComponent();
    Components::Physics::PhysicsComponent* getPhysicsComponent();
    Components::Video::VideoComponent* getVideoComponent();


  }; // End of Dream
} // End of Dream

#endif // DREAM_H
