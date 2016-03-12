
#ifdef __cplusplus
#include "../BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#endif

#include "dcwDefaultCollisionConfiguraion.h"

dcwDefaultCollisionConfiguration* dcwDefaultCollisionConfigurationCreate(void) {
	return reinterpret_cast<dcwDefaultCollisionConfiguration*>( 
			new btDefaultCollisionConfiguration());
}

void dcwDefaultCollisionConfigurationDestroy(dcwDefaultCollisionConfiguration* obj) {
	delete reinterpret_cast<btDefaultCollisionConfiguration*>(obj);
}


