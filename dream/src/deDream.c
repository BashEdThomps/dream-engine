#include "Dream.h"

dreamData* dreamCreate(int animation, ...) {
	va_list ap;
    int j;

    va_start(ap, count); //Requires the last fixed parameter (to get the address)

    for(j=0; j<count; j++)
	{
        int next = va_arg(ap, int);
		switch (next) {
			case ENABLE_SCENEGRAPH:
				data->scenegraph = dsgScenegraphCreate();
				break;
			case ENABLE_ANIMATION:
				data->animation = daAnimationCreate();
				break;
			case ENABLE_COLLISION_WORLD:
				data->collisionWorld = dcwCollisionWorldCreate();
				break;
			default:
				break;
		}

    va_end(ap);
}

void deDreamEngineDataDestroy(deDreamEngineData* data) {
	if (data->scenegraph != 0) {
		dsgScenegraphDestroy(data->scenegraph);
		data->scenegraph = 0;
	}

	if (data->collisionWorld != 0)
	dcwCollisionWorldDestroy(data->collisionWorld);
	data->collisionWorld = 0;

	free (data);
	data = 0;
}
