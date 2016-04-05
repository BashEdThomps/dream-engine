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

	bool Dream::loadProjectFromJsonFile(std::string path) {
		mProjectFileReader = new Util::FileReader(path);
		mProjectFileReader->readIntoStringStream();
		std::string projectJsonStr = mProjectFileReader->getContentsAsString();
		nlohmann::json projectJson = nlohmann::json::parse(projectJsonStr);
		setProject(new Project(projectJson));
		delete mProjectFileReader;
		return false;
	}
}
