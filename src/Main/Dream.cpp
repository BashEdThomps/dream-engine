#include "Dream.h"

namespace Dream {

	Dream::Dream() {
		mScriptingInterface = NULL;
		mAudioInterface     = NULL;
		mVideoInterface     = NULL;
		mPhysicsInterface   = NULL;

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
		nlohmann::json projectJson = nlohmann::json::parse(projectJsonStr);
		setProject(new Project(projectPath, projectJson));
		return isProjectLoaded();
	}

	bool Dream::loadFromArgumentParser(ArgumentParser *parser) {
		std::cout << "Dream: Loading from ArgumentParser" << std::endl;
		Util::FileReader *proj = new Util::FileReader(parser->getProjectFilePath());
		proj->readIntoStringStream();
		bool loadSuccess = loadProjectFromFileReader(parser->getProjectPath(), proj);
		delete proj;
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
			/*if (mPhysicsInterface == NULL) {
				mPhysicsInterface = new Physics::Bullet3Physics();
				if (!mPhysicsInterface->init())
				{
					std::cerr << "Unable to initialise Bullet3Physics." << std::endl;
					return false;
				}
			} else {
				std::cerr << "Unable to create Bullet3Physics. Physics interface allready exists." << std::endl;
				return false;
			}
			*/
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
			/*if (mVideoInterface == NULL) {
				mVideoInterface = new Video::VulkanVideo();
                mVideoInterface->setScreenName(mProject->getName());
				if (!mVideoInterface->init())
				{
					std::cerr << "Unable to initialise VulkanVideo." << std::endl;
					return false;
				}
			} else {
				std::cerr << "Unable to create VulkanVideo. Video interface allready exists." << std::endl;
				return false;
			}
			*/
			std::cerr << "VulkanVideo interface is not yet implemented" << std::endl;
			return false;
		}
		return false;
	}

	int Dream::runProject() {
		mError = !createInterfaces();
		mRunning = true;
		while(mRunning) {

			if (mError) {
				std::cerr << "Dream: A fatal error has occurred, exiting main loop" << std::endl;
				break;
			}

			updateInterfaces();
		}
		return 0;
	}

	void Dream::updateInterfaces(void) {
			if (mScriptingInterface != NULL) mScriptingInterface->update();
			if (mPhysicsInterface   != NULL) mPhysicsInterface->update();
			if (mAudioInterface     != NULL) mAudioInterface->update();
			if (mVideoInterface     != NULL) mVideoInterface->update();
	}
}
