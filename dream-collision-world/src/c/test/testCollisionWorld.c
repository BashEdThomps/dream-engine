#include <unit.h>

#include "../../cpp/CWrapper/dcwCollisionWorld.h"
#include "../../cpp/CWrapper/dcwBoxShape.h"
#include "../../cpp/CWrapper/dcwVector3.h"

#include "testCollisionWorld.h"

void testCollisionWorld(void) {
	unitTestHeading("Tsting CollisionWord");
	testCollisionWorldCreateWorld();
	testColWorldAddObject();
	testColWorldGetNextAvailableObjectInde();
	testColWorldRemoveObject();
	testColWorldUpdate();
	unitTestFooter("Finished Testing CollisionWord");
	return;
}

void testCollisionWorldCreateWorld(void) {
	unitPrintComment("Testing Create Collision World");
	dcwCollisionWorld* collisionWorld;
	collisionWorld = dcwCollisionWorldCreate();
	unitAssertNotNull("Checking collision world is not NULL", collisionWorld);
	return;
}

void testColWorldAddObject(void) {
	dcwCollisionWorld*  world = dcwCollisionWorldCreate();
	dcwVector3*         boxSize = dcwVector3Create(2.0f,2.0f,2.0f);
	dcwBoxShape*        boxShape = dcwBoxShapeCreate(boxShape);
	dcwCollisionObject* object = dcwCollisionObjectCreate();
	unitAssertNotZero("Add obbject to Collision",0);
	return;
}

void testColWorldGetNextAvailableObjectInde(void) {
	unitAssertFail("Get Next Available Object Index");
	return;
}

void testColWorldRemoveObject(void) {
	unitAssertFail("Remove Object");
	return;
}

void testColWorldUpdate(void) {
	unitAssertFail("Update World");
	return;
}
