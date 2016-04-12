#ifndef DREAM_H
#define DREAM_H

#include <iostream>
#include "Util/FileReader.h"
#include "Scene/Scene.h"
#include "ArgumentParser.h"
#include "Project.h"

namespace Dream {
	class Dream {
	protected:
		Util::FileReader* mProjectFileReader;
		Project*          mProject;
	public:
		Dream(void);
		~Dream(void);
		bool loadFromArgumentParser(ArgumentParser*);
		bool loadProjectFromJsonFile(std::string);
		Project* getProject(void);
		void setProject(Project*);
		int runProject();
	};
}

#endif // DREAM_H
