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

	 dcwScalar* eX = dcwScalarCreate(1.0f);
	 dcwScalar* eY = dcwScalarCreate(2.0f);
	 dcwScalar* eZ = dcwScalarCreate(4.0f);

	dcwVector3*         boxSize = dcwVector3Create(eX,eY,eZ);
	dcwBoxShape*        boxShape = dcwBoxShapeCreate(boxSize);
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
