#ifndef DREAM_H
#define DREAM_H

#include <iostream>

#include "ArgumentParser.h"

#include "../JSON/json.hpp"
#include "../Util/FileReader.h"
#include "../Scene/Scene.h"
#include "../Project/Project.h"

#include "../Plugins/PluginInterface.h"
#include "../Plugins/Audio/AudioInterface.h"
#include "../Plugins/Scripting/ScriptingInterface.h"
#include "../Plugins/Physics/PhysicsInterface.h"
#include "../Plugins/Input/InputInterface.h"
#include "../Plugins/Video/VideoInterface.h"

#include "../Plugins/Audio/OpenAL/OALAudio.h"
#include "../Plugins/Physics/Bullet/BulletPhysics.h"
#include "../Plugins/Scripting/v8/V8Scripting.h"
#include "../Plugins/Scripting/Chai/ChaiScripting.h"
#include "../Plugins/Video/OpenGL/OGLVideo.h"
#include "../Plugins/Animation/Dream/DreamAnimation.h"
#include "../Plugins/Input/GLFW/GLFWInput.h"

#include "../Asset/AssetInstance.h"
#include "../Asset/AssetManager.h"

namespace Dream {
	
	class Dream {
	protected:
		// Project
		Project::Project    *mProject;
		Scene::Scene        *mCurrentScene;
		Asset::AssetManager *mAssetManager;
		
		// Plugins
		Plugins::Audio::AudioInterface         *mAudioInterface;
		Plugins::Video::VideoInterface         *mVideoInterface;
		Plugins::Physics::PhysicsInterface     *mPhysicsInterface;
		Plugins::Scripting::ScriptingInterface *mScriptingInterface;
		Plugins::Input::InputInterface         *mInputInterface;
		Plugins::Animation::AnimationInterface *mAnimationInterface;
		
		// Engine Variables 
		bool mRunning;
		bool mError;

	public:
		Dream(void);
		~Dream(void);

		bool loadFromArgumentParser(ArgumentParser*);
		bool loadProjectFromFileReader(std::string projectPath, Util::FileReader*);
		bool loadScene(Scene::Scene*);

		Project::Project* getProject(void);
		void setProject(Project::Project*);
		int runProject();
		bool isProjectLoaded();

		bool createAssetManager();
		bool createInterfaces();
		bool createScriptingInterfaces();
		bool createPhysicsInterfaces();
		bool createVideoInterfaces();
		bool createAudioInterfaces();
		bool createInputInterfaces();
		bool createAnimationInterfaces();
		
		
		void updateAll();
	};
	
} // End of Dream

#endif // DREAM_H
