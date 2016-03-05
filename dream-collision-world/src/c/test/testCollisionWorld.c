#include <unit.h>

#include "../../cpp/CWrapper/dsgCollisionWorld.h"
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
	dsgCollisionWorld_t* collisionWorld;
	collisionWorld = dsgCollisionWorldCreate();
	unitAssertNotNull("Checking collision world is not NULL", collisionWorld);
	return;
}
void testColWorldAddObject(void) {
	unitAssertFail("Add Object");
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
