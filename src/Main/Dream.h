#ifndef DREAM_H
#define DREAM_H

#include <iostream>

#include "ArgumentParser.h"

#include "../JSON/json.hpp"
#include "../Util/FileReader.h"
#include "../Scene/Scene.h"
#include "../Project/Project.h"
#include "../Asset/AssetInstance.h"
#include "../Asset/AssetManager.h"
#include "../Plugins/PluginManager.h"

namespace Dream {
	
	class Dream {
	protected:
		Project::Project       *mProject;
		
		Asset::AssetManager    *mAssetManager;
		Plugins::PluginManager *mPluginManager;
		
		bool mRunning;

	public:
		Dream(void);
		~Dream(void);

		bool loadFromArgumentParser(ArgumentParser*);
		bool loadProjectFromFileReader(std::string projectPath, Util::FileReader*);
		bool loadScene(Scene::Scene*);

		Project::Project* getProject(void);
		void setProject(Project::Project*);
		int  runProject();
		bool isProjectLoaded();

		bool createAssetManager();
		bool createPluginManager();
		
		void update();
	};
	
} // End of Dream

#endif // DREAM_H
