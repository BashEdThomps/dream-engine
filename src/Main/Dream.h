#ifndef DREAM_H
#define DREAM_H

#include <iostream>

#include "ArgumentParser.h"

#include "../ExternalLibs/JSON/json.hpp"
#include "../Util/FileReader.h"
#include "../Scene/Scene.h"
#include "../Project/Project.h"
#include "../Asset/AssetInstance.h"


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
		bool loadScene(Scene::Scene*);
		static Project::Project* getProject();
		void setProject(Project::Project*);
		bool runProject();
		bool isProjectLoaded();
	}; // End of Dream
	
} // End of Dream

#endif // DREAM_H
