#include "PluginManager.h"

namespace Dream {
namespace Plugins {
	
	PluginManager::PluginManager(Project::Project* project) {
		mProject = project;
		
		mAnimationPlugin = NULL;
		mAudioPlugin     = NULL;
		mInputPlugin     = NULL;
		mScriptingPlugin = NULL;
		mPhysicsPlugin   = NULL;
		mVideoPlugin     = NULL;
		
	}
	
	PluginManager::~PluginManager() {
		destroyPlugins();
	}
	
	void PluginManager::destroyPlugins() {
		delete mAnimationPlugin;
		delete mAudioPlugin;
		delete mInputPlugin;
		delete mScriptingPlugin;
		delete mPhysicsPlugin;
		delete mVideoPlugin;
	}
	
	bool PluginManager::createPlugins() {
		std::cout << "PluginManager: Creating Plugins..." << std::endl;
        if(!createScriptingPlugin()) return false;
        if(!createAudioPlugin())     return false;
        if(!createPhysicsPlugin())   return false;
        if(!createVideoPlugin())     return false;
		if(!createInputPlugin())     return false;
		if(!createAnimationPlugin()) return false;
		std::cout << "PluginManager: Successfuly created Plugins." << std::endl;
        return true;
	}

	bool PluginManager::createScriptingPlugin() {
		// Chai
		if (mProject->isChaiEnabled()){
			if (mScriptingPlugin == NULL) {
				mScriptingPlugin = new Plugins::Scripting::Chai::ChaiScripting();
				if (mScriptingPlugin->init()) {
					return true;
				} else {
					std::cerr << "Unable to initialise ChaiScripting." << std::endl;
					return false;
				}
			} else {
				std::cerr << "Unable to create ChaiScripting. Scripting Plugin allready exists." << std::endl;
				return false;
			}
		}
		// V8
		else if (mProject->isV8Enabled()){
			if (mScriptingPlugin == NULL) {
				mScriptingPlugin = new Plugins::Scripting::V8::V8Scripting();
				if (mScriptingPlugin->init()) {
					return true;
				} else {
					std::cerr << "Unable to initialise V8Scripting." << std::endl;
					return false;
				}
			} else {
				std::cerr << "Unable to create V8Scripting. Scripting Plugin allready exists." << std::endl;
				return false;
			}
		}
		return false;
	}

	bool PluginManager::createAudioPlugin() {
		// OpenAL
		if (mProject->isOpenALEnabled()) {
			if (mAudioPlugin == NULL) {
				mAudioPlugin = new Plugins::Audio::OpenAL::OALAudio();
				if (mAudioPlugin->init()) {
					return true;
				} else {
					std::cerr << "Unable to initialise OALAudio." << std::endl;
					return false;
				}
			} else {
				std::cerr << "Unable to create OALAudio. Audio Plugin allready exists." << std::endl;
				return false;
			}
		}
		return false;
	}

	bool PluginManager::createPhysicsPlugin() {
		// Bullet 2
		if (mProject->isBullet2Enabled()) {
			if (mPhysicsPlugin == NULL) {
				mPhysicsPlugin = new Plugins::Physics::Bullet::BulletPhysics();
				if (mPhysicsPlugin->init()) {
					return true;
				} else {
					std::cerr << "Unable to initialise BulletPhysics." << std::endl;
					return false;
				}
			} else {
				std::cerr << "Unable to create BulletPhysics. Physics Plugin allready exists." << std::endl;
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

	bool PluginManager::createVideoPlugin() {
		// OpenGL
		if (mProject->isOpenGLEnabled()) {
			if (mVideoPlugin == NULL) {
				mVideoPlugin = new Plugins::Video::OpenGL::OGLVideo();
                mVideoPlugin->setScreenName(mProject->getName() + " :: " + mProject->getDescription());
				if (mVideoPlugin->init()) {
					return true;
				} else {
					std::cerr << "Unable to initialise OGLVideo." << std::endl;
					return false;
				}
			} else {
				std::cerr << "Unable to create OGLVideo. Video Plugin allready exists." << std::endl;
				return false;
			}
		}
		// Vulkan
		else if (mProject->isVulkanEnabled())  {
			std::cerr << "VulkanVideo Plugin is not yet implemented" << std::endl;
			return false;
		}
		return false;
	}

	bool PluginManager::createAnimationPlugin() {
		mAnimationPlugin = new Plugins::Animation::DreamAnimation();
		if (mAnimationPlugin->init()) {
			return true;
		} else {
			std::cerr << "PluginManager: Unable to initialise DreamAnimation." << std::endl;
			return false;
		}
		return true;
	}
	
	bool PluginManager::createInputPlugin() {
		if (mProject->isOpenGLEnabled()) {
			if (mInputPlugin == NULL) {
				mInputPlugin = new Plugins::Input::GLFW::GLFWInput();
				if (mInputPlugin->init()) {
					return true;
				} else {
					std::cerr << "PluginManager: Unable to initialise GLFWInputPlugin." << std::endl;
				}
			} else {
				std::cerr << "PluginManager: Cannot create GLFWInput. Input Plugin all ready exists." << std::endl;
				return false;
			}
			return true;
		}
		return false;
	}
	
	void PluginManager::update() {
		Scene::Scene* scene = mProject->getActiveScene();
		if (mInputPlugin     != NULL) mInputPlugin->update     (scene);
		if (mScriptingPlugin != NULL) mScriptingPlugin->update (scene);
		if (mPhysicsPlugin   != NULL) mPhysicsPlugin->update   (scene);
		if (mAnimationPlugin != NULL) mAnimationPlugin->update (scene);
		if (mAudioPlugin     != NULL) mAudioPlugin->update     (scene);
		if (mVideoPlugin     != NULL) mVideoPlugin->update     (scene);
	}
	
} // End of Plugins
} // End of Dream
