#include <unit.h>

#include "../../cpp/CWrapper/dcwCollisionWorld.h"
#include "../../cpp/CWrapper/dcwBoxShape.h"
#include "../../cpp/CWrapper/dcwVector3.h"

#include "testCollisionWorld.h"

void testCollisionWorld(void) {
	unitTestHeading("Tsting CollisionWord");
	testCollisionWorldCreateWorld();
	testColWorldAddObject();
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
	unitPrintComment("Collision World Add Object");
	dcwCollisionWorld*  world = dcwCollisionWorldCreate();

	 dcwScalar* eX = dcwScalarCreate(1.0f);
	 dcwScalar* eY = dcwScalarCreate(2.0f);
	 dcwScalar* eZ = dcwScalarCreate(4.0f);

	dcwVector3*         boxSize = dcwVector3Create(eX,eY,eZ);
	dcwBoxShape*        boxShape = dcwBoxShapeCreate(boxSize);
	dcwCollisionObject* object = dcwCollisionObjectCreate();
	dcwCollisionObjectSetCollisionBoxShape(object, boxShape);
	dcwCollisionWorldAddCollisionObject(world,object);

	unitAssertNotZero("Add obbject to Collision World",
	                  dcwCollisionWorldGetNumCollisionObjects(world));
	return;
}

void testColWorldRemoveObject(void) {
	unitPrintComment("Remove Object");
	dcwCollisionWorld*  world = dcwCollisionWorldCreate();

	 dcwScalar* eX = dcwScalarCreate(1.0f);
	 dcwScalar* eY = dcwScalarCreate(2.0f);
	 dcwScalar* eZ = dcwScalarCreate(4.0f);

	dcwVector3*         boxSize = dcwVector3Create(eX,eY,eZ);
	dcwBoxShape*        boxShape = dcwBoxShapeCreate(boxSize);
	dcwCollisionObject* object = dcwCollisionObjectCreate();
	dcwCollisionObjectSetCollisionBoxShape(object, boxShape);
	dcwCollisionWorldAddCollisionObject(world,object);

	unitAssertNotZero("Remove Object Adding obbject to Collision World",
	                  dcwCollisionWorldGetNumCollisionObjects(world));

	dcwCollisionWorldRemoveCollisionObject(world,object);

	unitAssertZero("Remove obbject to Collision World",
	                  dcwCollisionWorldGetNumCollisionObjects(world));

	dcwCollisionWorldDestroy(world);

	dcwCollisionObjectDestroy(object);

	dcwBoxShapeDestroy(boxShape);

	dcwVector3Destroy(boxSize);

	dcwScalarDestroy(eX);
	dcwScalarDestroy(eY);
	dcwScalarDestroy(eZ);
	return;
}

void testColWorldUpdate(void) {
	unitPrintComment("Update World");
	unitAssertFail("Update World");
	return;
}
