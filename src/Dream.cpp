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

	bool Dream::loadProjectFromFileReader(Util::FileReader* reader) {
		std::string projectJsonStr = reader->getContentsAsString();
		nlohmann::json projectJson = nlohmann::json::parse(projectJsonStr);
		setProject(new Project(projectJson));
		return false;
	}

	bool Dream::loadFromArgumentParser(ArgumentParser *parser) {
		return false;
	}

	int Dream::runProject() {
		return -1;
	}
}
