
#include "Dream.h"

namespace Dream {

  Project::Project *Dream::mProject = NULL;

  Dream::Dream() {
    mProject = NULL;
  }

  Dream::~Dream() {}

  Project::Project* Dream::getProject() {
    return mProject;
  }

  void Dream::setProject(Project::Project* project) {
    mProject = project;
  }

  bool Dream::isProjectLoaded() {
    return mProject != NULL;
  }

  bool Dream::loadProjectFromFileReader(std::string projectPath, Util::FileReader* reader) {
    std::cout << "Dream: Loading project from FileReader " << reader->getPath() << std::endl;
    std::string projectJsonStr = reader->getContentsAsString();
    #ifdef VERBOSE
      std::cout << "Dream: Read Project:" << std::endl << projectJsonStr << std::endl;
    #endif
    if (projectJsonStr.empty()) {
      std::cerr << "Dream: Loading Failed. Project Content is Empty" << std::endl;
      return false;
    }
    nlohmann::json projectJson = nlohmann::json::parse(projectJsonStr);
    setProject(new Project::Project(projectPath, projectJson));
    return isProjectLoaded();
  }

  bool Dream::loadFromArgumentParser(ArgumentParser *parser) {
    std::cout << "Dream: Loading from ArgumentParser" << std::endl;
    Util::FileReader *projectFileReader = new Util::FileReader(parser->getProjectFilePath());
    projectFileReader->readIntoStringStream();
    bool loadSuccess = loadProjectFromFileReader(parser->getProjectPath(), projectFileReader);
    delete projectFileReader;
    delete parser;
    return loadSuccess;
  }

  bool Dream::runProject() {
    return mProject->run();
  }
}
