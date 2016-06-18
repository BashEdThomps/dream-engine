#include "../Project/Project.h"
#include "ComponentManager.h"

namespace Dream {
namespace Components {
	
	ComponentManager::ComponentManager(Project::Project* project) {
		mProject            = project;
		mParallel           = false;
		mAnimationComponent = NULL;
		mAudioComponent     = NULL;
		mInputComponent     = NULL;
		mScriptingComponent = NULL;
		mPhysicsComponent   = NULL;
		mVideoComponent     = NULL;
		mDone               = false;
		
	}
	
	ComponentManager::~ComponentManager() {
		if (mParallel) {
			joinThreads();
		}
		destroyComponents();
	}
	
	void ComponentManager::destroyComponents() {
		delete mAnimationComponent;
		delete mAudioComponent;
		delete mInputComponent;
		delete mScriptingComponent;
		delete mPhysicsComponent;
		delete mVideoComponent;
	}
	
	bool ComponentManager::createComponents() {
		std::cout << "ComponentManager: Creating Components..." << std::endl;
		if(!createScriptingComponent()) return false;
		if(!createAudioComponent())     return false;
		if(!createPhysicsComponent())   return false;
		if(!createVideoComponent())     return false;
		if(!createInputComponent())     return false;
		if(!createAnimationComponent()) return false;
		std::cout << "ComponentManager: Successfuly created Components." << std::endl;
    return true;
	}

	bool ComponentManager::createScriptingComponent() {
		// ChaiScript
		if (mProject->isChaiEnabled()){
			if (mScriptingComponent == NULL) {
				mScriptingComponent = new Components::Scripting::Chai::ChaiScripting();
				if (!mScriptingComponent->init()) {
					std::cerr << "ComponentManager: Unable to initialise ChaiScripting." << std::endl;
					return false;
				}
			} else {
				std::cerr << "ComponentManager: Unable to create ChaiScripting. Scripting Component allready exists." << std::endl;
				return false;
			}
		}
		return mScriptingComponent != NULL;
	}

	bool ComponentManager::createAudioComponent() {
		// OpenAL
		if (mProject->isOpenALEnabled()) {
			if (mAudioComponent == NULL) {
				mAudioComponent = new Components::Audio::OpenAL::OpenALAudio();
				if (!mAudioComponent->init()) {
					std::cerr << "ComponentManager: Unable to initialise OpenALAudio." << std::endl;
					return false;
				}
			} else {
				std::cerr << "ComponentManager: Unable to create OpenALAudio. Audio Component allready exists." << std::endl;
				return false;
			}
		}
		return mAudioComponent != NULL;
	}

	bool ComponentManager::createPhysicsComponent() {
		// Bullet 2
		if (mProject->isBullet2Enabled()) {
			if (mPhysicsComponent == NULL) {
				mPhysicsComponent = new Components::Physics::Bullet::BulletPhysics();
				if (!mPhysicsComponent->init()){
					std::cerr << "ComponentManager: Unable to initialise BulletPhysics." << std::endl;
					return false;
				}
			} else {
				std::cerr << "ComponentManager: Unable to create BulletPhysics. Physics Component allready exists." << std::endl;
				return false;
			}
		}
		return mPhysicsComponent != NULL;
	}

	bool ComponentManager::createVideoComponent() {
		// OpenGL
		if (mProject->isOpenGLEnabled()) {
			if (mVideoComponent == NULL) {
				mVideoComponent = new Components::Video::OpenGL::OpenGLVideo();
        mVideoComponent->setScreenName(mProject->getName() + " :: " + mProject->getDescription());
				if (mVideoComponent->init()) {
					return true;
				} else {
					std::cerr << "ComponentManager: Unable to initialise OpenGLVideo." << std::endl;
					return false;
				}
			} else {
				std::cerr << "ComponentManager: Unable to create OpenGLVideo. Video Component allready exists." << std::endl;
				return false;
			}
		} else if (mProject->isVulkanEnabled()) {
			//mVideoComponent = new Components::Video::Vulkan::VulkanVideo();
			if (!mVideoComponent->init()) {
				std::cerr << "ComponentManager: Unable to initialise VulkanVideo" << std::endl;
				return false;
			}
		}
		return false;
	}

	bool ComponentManager::createAnimationComponent() {
		mAnimationComponent = new Components::Animation::DreamAnimation();
		if (mAnimationComponent->init()) {
			return true;
		} else {
			std::cerr << "ComponentManager: Unable to initialise DreamAnimation." << std::endl;
			return false;
		}
		return true;
	}
	
	bool ComponentManager::createInputComponent() {
		if (mProject->isOpenGLEnabled()) {
			if (mInputComponent == NULL) {
				mInputComponent = new Components::Input::GLFW::GLFWInput();
				try {
					Dream::Components::Video::OpenGL::OpenGLVideo* ogl   = dynamic_cast<Dream::Components::Video::OpenGL::OpenGLVideo*>(mVideoComponent);
					Dream::Components::Input::GLFW::GLFWInput*  input = dynamic_cast<Dream::Components::Input::GLFW::GLFWInput*>(mInputComponent);
					input->setWindow(ogl->getWindow());
				} catch (std::exception ex) {
					std::cerr << "ComponentManager: " << ex.what() << std::endl;
				}
				if (!mInputComponent->init()) {
					std::cerr << "ComponentManager: Unable to initialise GLFWInputComponent." << std::endl;
					return false;
				}
				return true;
			} else {
				std::cerr << "ComponentManager: Cannot create GLFWInput. Input Component all ready exists." << std::endl;
				return false;
			}
			return mInputComponent != NULL;
		}
		return false;
	}
	
	void ComponentManager::startThreads() {
		
		if (mAnimationComponent != NULL) {
			mAnimationComponentThread = new std::thread(&ComponentManager::componentThread, this,mAnimationComponent);
		}
		
		if (mPhysicsComponent != NULL) {
			mPhysicsComponentThread = new std::thread(&ComponentManager::componentThread, this,mPhysicsComponent);
		}
		
		if (mAudioComponent != NULL) {
			mAudioComponentThread = new std::thread(&ComponentManager::componentThread, this,mAudioComponent);
		}
	}
	
	void ComponentManager::componentThread(Components::ComponentInterface* component) {
		component->init();
		while (!mDone) {
			component->update(mActiveScene);
		}
	}
	
	void ComponentManager::joinThreads() {
		if (mAnimationComponentThread != NULL && mAnimationComponentThread->joinable()) {
			mAnimationComponentThread->join();
			delete mAnimationComponentThread;
			mAnimationComponentThread = NULL;
		}
		
		if (mAudioComponentThread != NULL && mAudioComponentThread->joinable()) {
			mAudioComponentThread->join();
			delete mAudioComponentThread;
			mAudioComponentThread = NULL;
		}
		
		if (mPhysicsComponentThread != NULL && mPhysicsComponentThread->joinable()) {
			mPhysicsComponentThread->join();
			delete mPhysicsComponentThread;
			mPhysicsComponentThread = NULL;
		}
	}
	
	bool ComponentManager::isDone() {
		return mDone;
	}
	
	void ComponentManager::update() {
		mDone = mVideoComponent->isWindowShouldCloseFlagSet();
		if(!mParallel) {
			mAudioComponent->update(mActiveScene);
			mPhysicsComponent->update(mActiveScene);
			mAnimationComponent->update(mActiveScene);
		}
		mInputComponent->update(mActiveScene);
		mScriptingComponent->update(mActiveScene);
		mVideoComponent->update(mActiveScene);
	}
	
	void ComponentManager::setActiveScene(Scene::Scene* scene) {
		mActiveScene = scene;
	}
	
	void ComponentManager::populatePhysicsWorld(std::vector<Scene::SceneObject*> soWithPhysicsObjects) {
		std::cout << "ComponentManager: Populating Physics World" << std::endl;
		try {
			std::vector<Scene::SceneObject*>::iterator soIter;
			Components::Physics::Bullet::BulletPhysics* bulletPhysicsComponent;
			bulletPhysicsComponent = dynamic_cast<Components::Physics::Bullet::BulletPhysics*>(mPhysicsComponent);
			for (soIter = soWithPhysicsObjects.begin(); soIter != soWithPhysicsObjects.end(); soIter++) {
				#ifdef VERBOSE
					std::cout << "ComponentManager: Adding SceneObject " << (*soIter)->getuid()
				            << " to PhysicsComponent World" << std::endl;
				#endif
				Asset::Instances::Physics::Bullet::PhysicsObjectInstance* physicsObject;
				physicsObject = dynamic_cast<Asset::Instances::Physics::Bullet::PhysicsObjectInstance*>((*soIter)->getPhysicsObjectAssetInstance());
				bulletPhysicsComponent->addPhysicsObjectInstance(physicsObject);
			}
		} catch (const std::exception &ex) {
			std::cerr << "ComponentManager: Exception thrown while populating physics world..."
			          << std::endl << ex.what() << std::endl;;
		}
	}
	
	Components::Audio::AudioComponentInterface* ComponentManager::getAudioComponent() {
		return mAudioComponent;
	}
	
	Components::Video::VideoComponentInterface* ComponentManager::getVideoComponent() {
		return mVideoComponent;
	}
	
	bool ComponentManager::isParallel() {
		return mParallel;
	}
	
	void ComponentManager::setParallel(bool parallel) {
		mParallel = parallel;
	}
	
} // End of Components
} // End of Dream
