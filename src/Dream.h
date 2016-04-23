#ifndef DREAM_H
#define DREAM_H

#include <iostream>
#include "Util/FileReader.h"
#include "Scene/Scene.h"
#include "ArgumentParser.h"
#include "Project.h"
#include "Audio/AudioInterface.h"
#include "Physics/PhysicsInterface.h"
#include "Video/VideoInterface.h"


namespace Dream {
	class Dream {
	protected:
		Project *mProject;
		Audio::AudioInterface     *mAudioInterface;
		Video::VideoInterface     *mVideoInterface;
		Physics::PhysicsInterface *mPhysicsInterface;

	public:
		Dream(void);
		~Dream(void);
		bool loadFromArgumentParser(ArgumentParser*);
		bool loadProjectFromFileReader(Util::FileReader*);
		Project* getProject(void);
		void setProject(Project*);
		int runProject();
		bool isProjectLoaded();
	};
}

#endif // DREAM_H
