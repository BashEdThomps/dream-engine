#ifndef ComponentManager_hpp
#define ComponentManager_hpp

#include <iostream>

#include "../Components/ComponentInterface.h"
#include "../Components/Animation/AnimationComponentInterface.h"
#include "../Components/Audio/AudioComponentInterface.h"
#include "../Components/Scripting/ScriptingComponentInterface.h"
#include "../Components/Physics/PhysicsComponentInterface.h"
#include "../Components/Input/InputComponentInterface.h"
#include "../Components/Video/VideoComponentInterface.h"
#include "../Components/Audio/OpenAL/OpenALAudio.h"
#include "../Components/Physics/Bullet/BulletPhysics.h"
#include "../Components/Scripting/Chai/ChaiScripting.h"
#include "../Components/Video/OpenGL/OpenGLVideo.h"
#include "../Components/Video/Vulkan/VulkanVideo.h"
#include "../Components/Animation/Dream/DreamAnimation.h"
#include "../Components/Input/GLFW/GLFWInput.h"

#include <thread>

namespace Dream {

// Project Forward Declaration
namespace Project {
	class Project;
}
	
namespace Components {
	
	class ComponentManager {
	private:
		Project::Project *mProject;
		Scene::Scene     *mActiveScene;
		bool              mDone;
		bool              mParallel;
		
		Components::Audio::AudioComponentInterface *mAudioComponent;
		Components::Video::VideoComponentInterface *mVideoComponent;
		Components::Physics::PhysicsComponentInterface *mPhysicsComponent;
		Components::Scripting::ScriptingComponentInterface *mScriptingComponent;
		Components::Input::InputComponentInterface *mInputComponent;
		Components::Animation::AnimationComponentInterface *mAnimationComponent;
		
		std::thread *mAudioComponentThread;
		std::thread *mPhysicsComponentThread;
		std::thread *mAnimationComponentThread;
		
	public:
		ComponentManager(Project::Project*);
		~ComponentManager();
		bool createComponents();
		void destroyComponents();
		bool createAnimationComponent();
		bool createAudioComponent();
		bool createInputComponent();
		bool createPhysicsComponent();
		bool createScriptingComponent();
		bool createVideoComponent();
		void startThreads();
		void joinThreads();
		bool isDone();
		void populatePhysicsWorld(std::vector<Scene::SceneObject*>);
		
		Components::Animation::AnimationComponentInterface* getAnimationComponent();
		Components::Audio::AudioComponentInterface* getAudioComponent();
		Components::Input::InputComponentInterface* getInputComponent();
		Components::Physics::PhysicsComponentInterface* getPhysicsComponent();
		Components::Video::VideoComponentInterface* getVideoComponent();
		
		void componentThread(Components::ComponentInterface*);
		void update();
		void setActiveScene(Scene::Scene*);
		void setParallel(bool);
		bool isParallel();
	}; // End of ComponentManager
	
} // End of Components
} // End Dream

#endif /* ComponentManager_hpp */
