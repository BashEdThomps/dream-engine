#ifndef DREAM_H
#define DREAM_H

#include "Scene/Scene.h"
#include "Project.h"

namespace Dream {
	class Dream {
	protected:
		Project* mProject;
	public:
		Dream(void);
		~Dream(void);
		Project* getProject(void);
		void setProject(Project*);
	};
}

#endif // DREAM_H
