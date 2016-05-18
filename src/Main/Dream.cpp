#include "Dream.h"

namespace Dream {

	Dream::Dream() {
		mScriptingInterface = NULL;
		mAudioInterface     = NULL;
		mVideoInterface     = NULL;
		mPhysicsInterface   = NULL;
		
		mCurrentScene       = NULL;
		mProject            = NULL;
		
		mRunning = false;
		mError   = false;
	}

	Dream::~Dream() {}

	Project* Dream::getProject() {
		return mProject;
	}

	void Dream::setProject(Project* project) {
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
		setProject(new Project(projectPath, projectJson));
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

	bool Dream::createInterfaces() {
		if (!isProjectLoaded()) {
			std::cerr << "Dream: Cannot load interfaces, no project is loaded" << std::endl;
            return false;
		}

		std::cout << "Dream: Creating Interfaces..." << std::endl;

        if(!createScriptingInterfaces()) return false;
        if(!createAudioInterfaces())     return false;
        if(!createPhysicsInterfaces())   return false;
        if(!createVideoInterfaces())     return false;
		if(!createInputInterfaces())     return false;
		if(!createAnimationInterfaces()) return false;

		std::cout << "Dream: Successfuly created interfaces." << std::endl;
        return true;
	}

	bool Dream::createScriptingInterfaces() {
		// Chai
		if (mProject->isChaiEnabled()){
			if (mScriptingInterface == NULL) {
				mScriptingInterface = new Plugins::Scripting::Chai::ChaiScripting();
				if (mScriptingInterface->init()) {
					return true;
				} else {
					std::cerr << "Unable to initialise ChaiScripting." << std::endl;
					return false;
				}
			} else {
				std::cerr << "Unable to create ChaiScripting. Scripting interface allready exists." << std::endl;
				return false;
			}
		}
		// V8
		else if (mProject->isV8Enabled()){
			if (mScriptingInterface == NULL) {
				mScriptingInterface = new Plugins::Scripting::V8::V8Scripting();
				if (mScriptingInterface->init()) {
					return true;
				} else {
					std::cerr << "Unable to initialise V8Scripting." << std::endl;
					return false;
				}
			} else {
				std::cerr << "Unable to create V8Scripting. Scripting interface allready exists." << std::endl;
				return false;
			}
		}
		return false;
	}

	bool Dream::createAudioInterfaces() {
		// OpenAL
		if (mProject->isOpenALEnabled()) {
			if (mAudioInterface == NULL) {
				mAudioInterface = new Plugins::Audio::OpenAL::OALAudio();
				if (mAudioInterface->init()) {
					return true;
				} else {
					std::cerr << "Unable to initialise OALAudio." << std::endl;
					return false;
				}
			} else {
				std::cerr << "Unable to create OALAudio. Audio interface allready exists." << std::endl;
				return false;
			}
		}
		return false;
	}

	bool Dream::createPhysicsInterfaces() {
		// Bullet 2
		if (mProject->isBullet2Enabled()) {
			if (mPhysicsInterface == NULL) {
				mPhysicsInterface = new Plugins::Physics::Bullet::BulletPhysics();
				if (mPhysicsInterface->init()) {
					return true;
				} else {
					std::cerr << "Unable to initialise BulletPhysics." << std::endl;
					return false;
				}
			} else {
				std::cerr << "Unable to create BulletPhysics. Physics interface allready exists." << std::endl;
				return false;
			}
		}
		// Bullet3
		else if (mProject->isBullet3Enabled()) {
			std::cerr << "Bullet3Physics is not yet implemented." << std::endl;
			return false;
		}
		return false;
	}

	bool Dream::createVideoInterfaces() {
		// OpenGL
		if (mProject->isOpenGLEnabled()) {
			if (mVideoInterface == NULL) {
				mVideoInterface = new Plugins::Video::OpenGL::OGLVideo();
                mVideoInterface->setScreenName(mProject->getName());
				if (mVideoInterface->init()) {
					return true;
				} else {
					std::cerr << "Unable to initialise OGLVideo." << std::endl;
					return false;
				}
			} else {
				std::cerr << "Unable to create OGLVideo. Video interface allready exists." << std::endl;
				return false;
			}
		}
		// Vulkan
		else if (mProject->isVulkanEnabled())  {
			std::cerr << "VulkanVideo interface is not yet implemented" << std::endl;
			return false;
		}
		return false;
	}

	bool Dream::createAnimationInterfaces() {
		mAnimationInterface = new Plugins::Animation::DreamAnimation();
		if (mAnimationInterface->init()) {
			return true;
		} else {
			std::cerr << "Dream: Unable to initialise DreamAnimation." << std::endl;
			return false;
		}
		return true;
	}
	
	bool Dream::createInputInterfaces() {
		if (mProject->isOpenGLEnabled()) {
			if (mInputInterface == NULL) {
				mInputInterface = new Plugins::Input::GLFWInput();
				if (mInputInterface->init()) {
					return true;
				} else {
					std::cerr << "Dream: Unable to initialise GLFWInputInterface." << std::endl;
				}
			} else {
				std::cerr << "Dream: Cannot create GLFWInput. Input Interface all ready exists." << std::endl;
				return false;
			}
			return true;
		}
		return false;
	}
	
	bool Dream::loadScene(Scene::Scene* scene) {
		std::cout << "Dream: Loading Scene " << scene->getName() << std::endl;
		
		mCurrentScene = scene;
		
		if (mCurrentScene == NULL) {
			std::cerr << "Dream: Unable to find startup scene. Cannot Continue." << std::endl;
			return false;
		}
		
		if (!mCurrentScene->init()) {
			std::cerr << "Dream: Unable to initialise Current Scene." << std::endl;
			return false;
		}
	
		if (!createAssetInstances()) {
			std::cerr << "Dream: Fatal Error, unable to create asset instances" << std::endl;
			return false;
		}
		
		return true;
	}
	
	bool Dream::createAssetInstances() {
		std::vector<Scene::SceneObject*> scenegraph = mCurrentScene->getScenegraphVector();
		std::vector<Scene::SceneObject*>::iterator scenegraphIterator;
		
		std::vector<std::string> assetInstanceUUIDVector;
		std::vector<std::string>::iterator assetInstanceUUIDIterator;
		
		for (scenegraphIterator = scenegraph.begin();
			 scenegraphIterator != scenegraph.end(); scenegraphIterator++) {
			
			Scene::SceneObject* currentSceneObject = (*scenegraphIterator);
			assetInstanceUUIDVector = currentSceneObject->getAssetInstanceUUIDVector();
			
			for (assetInstanceUUIDIterator = assetInstanceUUIDVector.begin();
				 assetInstanceUUIDIterator != assetInstanceUUIDVector.end();
				 assetInstanceUUIDIterator++) {
				
				std::string assetDefinitionUUID = *assetInstanceUUIDIterator;
				Asset::Instance::AssetInstance* newAsset = createAssetInstanceFromUUID(assetDefinitionUUID);
				if (newAsset == NULL) {
					std::cerr << "Dream: Unable to instanciate asset " << assetDefinitionUUID << std::endl;
					return false;
				} else {
					currentSceneObject->addAssetInstance(newAsset);
				}
			}
		}
		return true;
	}
	
	Asset::Instance::AssetInstance* Dream::createAssetInstanceFromUUID(std::string uuid) {
		Asset::AssetDefinition* assetDefinition = mProject->getAssetDefinitionByUUID(uuid);
		return createAssetInstance(assetDefinition);
	}
	
	Asset::Instance::AssetInstance* Dream::createAssetInstance(Asset::AssetDefinition* definition) {
		Asset::Instance::AssetInstance* retval = NULL;
		if (definition->isTypeAnimation()) {
			retval = createAnimationAssetInstance(definition);
		} else if (definition->isTypeAudio()) {
			retval = createAudioAssetInstance(definition);
		} else if (definition->isTypeModel()) {
			retval = createModelAssetInstance(definition);
		} else if (definition->isTypeScript()){
			retval = createScriptAssetInstance(definition);
		}
		return retval;
	}
	
	Asset::Instance::AssetInstance* Dream::createAnimationAssetInstance(Asset::AssetDefinition* definition) {
		Asset::Instance::AssetInstance* retval = NULL;
		if (definition->isAnimationFormatDream()) {
			retval = new Asset::Instance::Animation::AnimationInstance(definition);
		}
		return retval;
	}
	
	
	Asset::Instance::AssetInstance* Dream::createAudioAssetInstance(Asset::AssetDefinition* definition) {
		Asset::Instance::AssetInstance* retval = NULL;
		if (definition->isAudioFormatOgg()) {
			retval = new Asset::Instance::Audio::Ogg::OggAudioInstance(definition);
		} else if (definition->isAudioFormatWav()) {
			retval = new Asset::Instance::Audio::Wav::WavAudioInstance(definition);
		}
		return retval;
	}
	
	Asset::Instance::AssetInstance* Dream::createModelAssetInstance(Asset::AssetDefinition* definition) {
		Asset::Instance::AssetInstance* retval = NULL;
		if (definition->isModelFormatWaveFront()) {
			retval = new Asset::Instance::Model::WaveFront::ObjModelInstance(definition);
		}
		return retval;
	}
	
	Asset::Instance::AssetInstance* Dream::createScriptAssetInstance(Asset::AssetDefinition* definition) {
		Asset::Instance::AssetInstance* retval = NULL;
		if (definition->isScriptFormatJavaScript()) {
			retval = new Asset::Instance::Script::JavaScript::JavaScriptInstance(definition);
		} else if (definition->isScriptFormatChaiScript()) {
			retval = new Asset::Instance::Script::ChaiScript::ChaiScriptInstance(definition,
						dynamic_cast<Plugins::Scripting::Chai::ChaiScripting*>(mScriptingInterface)->getChaiEngine());
		}
		return retval;
	}
	
	int Dream::runProject() {
		mError = !createInterfaces();
		
		if (!loadScene(mProject->getStartupScene())) {
			std::cerr << "Dream: Unable to load scene." << std::endl;
			return 1;
		}
	
		mRunning = true;
		std::cout << "Dream: Starting Scene - " << mCurrentScene->getName() << " (" << mCurrentScene->getUUID() << ")" << std::endl;
		while(mRunning) {
			if (mError) {
				std::cerr << "Dream: A fatal error has occurred, exiting main loop" << std::endl;
				return 1;
			}
			updateAll();
		}
		return 0;
	}

	void Dream::updateAll(void) {
		if (mInputInterface     != NULL) mInputInterface->update     (mCurrentScene);
		if (mScriptingInterface != NULL) mScriptingInterface->update (mCurrentScene);
		if (mPhysicsInterface   != NULL) mPhysicsInterface->update   (mCurrentScene);
		if (mAnimationInterface != NULL) mAnimationInterface->update (mCurrentScene);
		if (mAudioInterface     != NULL) mAudioInterface->update     (mCurrentScene);
		if (mVideoInterface     != NULL) mVideoInterface->update     (mCurrentScene);
	}
}
