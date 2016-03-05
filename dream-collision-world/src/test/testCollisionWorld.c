#include <unit.h>
#include "testCollisionWorld.h"

void testCollisionWorld(void) {
	unitTestHeading("Tsting CollisionWord");
	testColWorldAddObject();
	testColWorldGetNextAvailableObjectInde();
	testColWorldRemoveObject();
	testColWorldUpdate();
	unitTestFooter("Finished Testing CollisionWord");
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


