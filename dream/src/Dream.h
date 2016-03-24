#ifndef DREAM_H
#define DREAM_H

#include "../../dream-scenegraph/src/Scenegraph.h"
#include "../../dream-animation/src/Animation.h"

namespace Dream {
	class DreamInstance {
	protected:
		Scenegraph      *mSceneGraph;
		Animation       *mAnimations;
		btDynamicsWorld *mDynamicsWorld;
	public:
        Dream(int, ...);
		~Dream();
	};
}


#endif // DREAM_H
