#ifndef DREAM_H
#define DREAM_H

#include <iostream>
#include "Util/FileReader.h"
#include "Scene/Scene.h"
#include "ArgumentParser.h"
#include "Project.h"

#include "PluginInterfaces/Interface.h"
#include "PluginInterfaces/AudioInterface.h"
#include "Plugins/Audio/OpenAL/OALAudio.h"

#include "PluginInterfaces/PhysicsInterface.h"
#include "Plugins/Physics/Bullet/BulletPhysics.h"

#include "PluginInterfaces/VideoInterface.h"
#include "Plugins/Video/OpenGL/OGLVideo.h"

#include "PluginInterfaces/ScriptingInterface.h"
#include "Plugins/Scripting/v8/V8Scripting.h"
#include "Plugins/Scripting/Chai/ChaiScripting.h"

namespace Dream {
	class Dream {
	protected:
		// Project
		Project *mProject;
		// Plugins
		Plugins::Audio::AudioInterface         *mAudioInterface;
		Plugins::Video::VideoInterface         *mVideoInterface;
		Plugins::Physics::PhysicsInterface     *mPhysicsInterface;
		Plugins::Scripting::ScriptingInterface *mScriptingInterface;
		// Engine Running
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
