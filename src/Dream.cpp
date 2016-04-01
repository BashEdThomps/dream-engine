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
}
