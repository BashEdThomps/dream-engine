#include "deDreamEngine.h"


void deDreamEngineDataInit(deDreamEngineData* data) {
	data->scenegraph = dsgScenegraphInit();
	data->collisionWorld = dcwCollisionWorldCreate();
}

void deDreamEngineDataDestroy(deDreamEngineData* data) {
	free (data->scenegraph);
	data->scenegraph = 0;

	dcwCollisionWorldDestroy(data->collisionWorld);
	data->collisionWorld = 0;

	free (data);
	data = 0;
}
