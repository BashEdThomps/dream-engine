#ifndef DREAM_H
#define DREAM_H

#include <iostream>

#include "ArgumentParser.h"

#include "../JSON/json.hpp"
#include "../Util/FileReader.h"
#include "../Scene/Scene.h"
#include "../Project/Project.h"

#include "../Plugins/Interfaces/Interface.h"
#include "../Plugins/Interfaces/AudioInterface.h"
#include "../Plugins/Interfaces/ScriptingInterface.h"
#include "../Plugins/Interfaces/PhysicsInterface.h"
#include "../Plugins/Interfaces/VideoInterface.h"

#include "../Plugins/Audio/OpenAL/OALAudio.h"

#include "../Plugins/Physics/Bullet/BulletPhysics.h"

#include "../Plugins/Scripting/v8/V8Scripting.h"
#include "../Plugins/Scripting/Chai/ChaiScripting.h"

#include "../Plugins/Video/OpenGL/OGLVideo.h"

namespace Dream {
	class Dream {
	protected:
		// Project
		Project      *mProject;
		Scene::Scene *mCurrentScene;
		
		// Plugins
		Plugins::Audio::AudioInterface         *mAudioInterface;
		Plugins::Video::VideoInterface         *mVideoInterface;
		Plugins::Physics::PhysicsInterface     *mPhysicsInterface;
		Plugins::Scripting::ScriptingInterface *mScriptingInterface;
		
		// Engine Variables 
		bool mRunning;
		bool mError;

	public:
		Dream(void);
		~Dream(void);

		bool loadFromArgumentParser(ArgumentParser*);
		bool loadProjectFromFileReader(std::string projectPath, Util::FileReader*);

		Project* getProject(void);
		void setProject(Project*);
		int runProject();
		bool isProjectLoaded();

		bool createInterfaces();
		bool createScriptingInterfaces();
		bool createPhysicsInterfaces();
		bool createVideoInterfaces();
		bool createAudioInterfaces();
		void updateInterfaces();
	};
}

#endif // DREAM_H
