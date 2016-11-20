#ifndef DREAM_H
#define DREAM_H

#include <iostream>

#include "ArgumentParser.h"

#include "json.hpp"
#include "FileReader.h"
#include "Scene.h"
#include "Project.h"
#include "AssetInstance.h"


namespace Dream {
  class Dream {
  private:
    static Project::Project *mProject;
    bool   mRunning;
  public:
    Dream(void);
    ~Dream(void);
    bool loadFromArgumentParser(ArgumentParser*);
    bool loadProjectFromFileReader(std::string projectPath, Util::FileReader*);
    bool loadScene(Scene*);
    static Project::Project* getProject();
    void setProject(Project::Project*);
    bool runProject();
    bool isProjectLoaded();
  }; // End of Dream
} // End of Dream

#endif // DREAM_H
