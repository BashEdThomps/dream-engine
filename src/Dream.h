#ifndef DREAM_H
#define DREAM_H

#include <iostream>
#include "Util/FileReader.h"
#include "Scene/Scene.h"
#include "ArgumentParser.h"
#include "Project.h"

#include "Interfaces/Interface.h"
#include "Interfaces/AudioInterface.h"
#include "Audio/OALAudio.h"

#include "Interfaces/PhysicsInterface.h"
#include "Physics/BulletPhysics.h"

#include "Interfaces/VideoInterface.h"
#include "Video/OGLVideo.h"

#include "Interfaces/ScriptingInterface.h"
#include "Scripting/V8Scripting.h"

namespace Dream {
	class Dream {
	protected:
		Project                       *mProject;
		Audio::AudioInterface         *mAudioInterface;
		Video::VideoInterface         *mVideoInterface;
		Physics::PhysicsInterface     *mPhysicsInterface;
		Scripting::ScriptingInterface *mScriptingInterface;
		bool mRunning;

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
