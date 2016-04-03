#include "Dream.h"
#include "Project.h"

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
		std::string projectJson = mProjectFileReader->getContentsAsString();
		setProject(new Project(projectJson));
		delete mProjectFileReader;
		return false;
	}
}
