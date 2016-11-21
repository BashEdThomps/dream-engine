#ifndef DREAM_H
#define DREAM_H

#include <iostream>

#include "ArgumentParser.h"

#include "json.hpp"
#include "Time.h"
#include "FileReader.h"
#include "Scene.h"
#include "Project.h"
#include "AssetInstance.h"
#include "AssetManager.h"
#include "Components/ComponentManager.h"
#include "Components/Video/Camera.h"


namespace Dream {
  class Dream {
  private:
    Time *mTime;
    Project *mProject;
    AssetManager *mAssetManager;
    Components::ComponentManager *mComponentManager;
    bool mDone;
    Components::Video::Camera *mCamera;
    Scene* mActiveScene;
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

    bool createComponentManager();
    Components::ComponentManager* getComponentManager();
    bool run();
    void setTime(Time*);
    void setDone(bool);

    void   setActiveScene(Scene*);
    Scene *getActiveScene();
    bool   hasActiveScene();
    bool   initActiveScene();

  }; // End of Dream
} // End of Dream

#endif // DREAM_H
