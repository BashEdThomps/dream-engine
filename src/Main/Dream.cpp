#include "Dream.h"

namespace Dream {

	Dream::Dream() {
	
		mProject       = NULL;
		mAssetManager  = NULL;
		mPluginManager = NULL;
		
		mRunning = false;
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
		std::cout << "Dream: Read Project:" << std::endl << projectJsonStr << std::endl;
		nlohmann::json projectJson = nlohmann::json::parse(projectJsonStr);
		setProject(new Project::Project(projectPath, projectJson));
		createPluginManager();
		createAssetManager();
		return isProjectLoaded();
	}
	
	bool Dream::createPluginManager() {
		if (mPluginManager != NULL) {
			std::cout << "Dream: Destroying existing PluginManager." << std::endl;
			delete mPluginManager;
		}
		if (mProject == NULL) {
			std::cerr << "Dream: Cannot create Plugin Manager without Project." << std::endl;
			return false;
		}
		mPluginManager = new Plugins::PluginManager(mProject);
		return mPluginManager != NULL;
	}
	
	bool Dream::createAssetManager() {
		if (mProject == NULL) {
			std::cerr << "Dream: Cannot create Asset Manager without a Project." << std::endl;
			return false;
		}
		
		if (mAssetManager != NULL) {
			std::cout << "Dream: Destroying existing Asset Manager." << std::endl;
			delete mAssetManager;
		}
		
		mAssetManager = new Asset::AssetManager(getProject());
		return mAssetManager != NULL;
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

	
	
	bool Dream::loadScene(Scene::Scene* scene) {
		std::cout << "Dream: Loading Scene " << scene->getName() << std::endl;
		
		mProject->setActiveScene(scene);
		
		if (!mProject->hasActiveScene()) {
			std::cerr << "Dream: Unable to find active scene. Cannot Continue." << std::endl;
			return false;
		}
		
		if (!mProject->initActiveScene()) {
			std::cerr << "Dream: Unable to initialise Current Scene." << std::endl;
			return false;
		}
	
		if (!mAssetManager->createAssetInstances()) {
			std::cerr << "Dream: Fatal Error, unable to create asset instances" << std::endl;
			return false;
		}
		
		return true;
	}
	
	int Dream::runProject() {
		if(!mPluginManager->createPlugins()){
			std::cerr << "Dream: Unable to create interfaces." << std::endl;
			return 1;
		}
		
		if (!loadScene(mProject->getStartupScene())) {
			std::cerr << "Dream: unable to load startup scene." << std::endl;
			return 1;
		}
	
		mRunning = true;
		std::cout << "Dream: Starting Scene - " << mProject->getActiveScene()->getName()
		          << " (" << mProject->getActiveScene()->getUUID() << ")" << std::endl;
		
		while(mRunning) {
			update();
		}
		
		return 0;
	}

	void Dream::update(void) {
		mPluginManager->update();
	}
}
