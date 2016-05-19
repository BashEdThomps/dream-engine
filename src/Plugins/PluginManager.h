#ifndef PluginManager_hpp
#define PluginManager_hpp

#include <iostream>
#include "../Project/Project.h"

#include "../Plugins/PluginInterface.h"
#include "../Plugins/Animation/AnimationPluginInterface.h"
#include "../Plugins/Audio/AudioPluginInterface.h"
#include "../Plugins/Scripting/ScriptingPluginInterface.h"
#include "../Plugins/Physics/PhysicsPluginInterface.h"
#include "../Plugins/Input/InputPluginInterface.h"
#include "../Plugins/Video/VideoPluginInterface.h"

#include "../Plugins/Audio/OpenAL/OALAudio.h"
#include "../Plugins/Physics/Bullet/BulletPhysics.h"
#include "../Plugins/Scripting/v8/V8Scripting.h"
#include "../Plugins/Scripting/Chai/ChaiScripting.h"
#include "../Plugins/Video/OpenGL/OGLVideo.h"
#include "../Plugins/Animation/Dream/DreamAnimation.h"
#include "../Plugins/Input/GLFW/GLFWInput.h"


namespace Dream {
namespace Plugins {
	
	class PluginManager {
	private:
		Project::Project* mProject;
		
		// Plugins
		Plugins::Audio::AudioPluginInterface         *mAudioPlugin;
		Plugins::Video::VideoPluginInterface         *mVideoPlugin;
		Plugins::Physics::PhysicsPluginInterface     *mPhysicsPlugin;
		Plugins::Scripting::ScriptingPluginInterface *mScriptingPlugin;
		Plugins::Input::InputPluginInterface         *mInputPlugin;
		Plugins::Animation::AnimationPluginInterface *mAnimationPlugin;
		
	public:
		PluginManager(Project::Project*);
		~PluginManager();
		
		bool createPlugins();
		void destroyPlugins();
		
		bool createAnimationPlugin();
		bool createAudioPlugin();
		bool createInputPlugin();
		bool createPhysicsPlugin();
		bool createScriptingPlugin();
		bool createVideoPlugin();
		
		void update();
		
	}; // End of PluginManager
	
} // End of Plugins
} // End Dream

#endif /* PluginManager_hpp */
