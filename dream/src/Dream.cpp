#include "Dream.h"

Dream::DreamInstance(int animation, ...) {
	va_list ap;
    int j;

    va_start(ap, count); //Requires the last fixed parameter (to get the address)

    for(j=0; j<count; j++)
	{
        int next = va_arg(ap, int);
		switch (next) {
			case ENABLE_SCENEGRAPH:
				scenegraph = new DreamSceneGraph::SceneGraph();
				break;
			case ENABLE_BULLET:
				dynamicsWorld = new btDynamicsWorld();
			default:
				break;
		}

    va_end(ap);
}

Dream::~DreamInstance() {
	if (mSceneGraph != 0) {
		delete mSceneGraph;
		mSceneGraph = 0;
	}

	if (mDynamicsWorld != 0) {
		delete mDynamicsWorld;
		mDynamicsWorld = 0;
	}
}
