#include "Dream.h"
#include "Project.h"
#include "JSON/json.hpp"

namespace Dream {
	Dream::Dream() {
	}

	Dream::~Dream() {
	}

	Project* Dream::getProject() {
		return mProject;
	}

	void Dream::setProject(Project* project) {
		mProject = project;
	}

	bool Dream::isProjectLoaded() {
		return mProject != NULL;
	}

	bool Dream::loadProjectFromFileReader(Util::FileReader* reader) {
		std::cout << "Dream: Loading project from FileReader " << reader->getPath() << std::endl;
		std::string projectJsonStr = reader->getContentsAsString();
		nlohmann::json projectJson = nlohmann::json::parse(projectJsonStr);
		setProject(new Project(projectJson));
		return isProjectLoaded();
	}

	bool Dream::loadFromArgumentParser(ArgumentParser *parser) {
		std::cout << "Dream: Loading from ArgumentParser" << std::endl;
		Util::FileReader *proj = new Util::FileReader(parser->getProjectFilePath());
		proj->readIntoStringStream();
		bool loadSuccess = loadProjectFromFileReader(proj);
		delete proj;
		return loadSuccess;
	}

	bool Dream::createInterfaces() {
		return false;
	}

	int Dream::runProject() {
		return 0;
	}
}
