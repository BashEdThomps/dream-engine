#include "../../../../unit/src/unit.h"

#include "../../cpp/CWrapper/dcwCollisionWorld.h"
#include "../../cpp/CWrapper/dcwBoxShape.h"
#include "../../cpp/CWrapper/dcwVector3.h"
#include "../../cpp/CWrapper/dcwTransform.h"
#include "../../cpp/CWrapper/dcwSortedOverlappingPairCache.h"

#include "testCollisionWorld.h"

void testCollisionWorld(void) {
	unitTestHeading("Tsting CollisionWord");
	testCollisionWorldCreateWorld();
	testColWorldAddObject();
	testColWorldRemoveObject();
	testColWorldPerformDiscreteCollisionDetection();
	testColWorldTwoBoxCollision(); 
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

void testColWorldPerformDiscreteCollisionDetection(void) {
	unitPrintComment("Perform Discrete Collision Detection");

	unitPrintComment("PDCD: Collision World Add Objects");
	dcwCollisionWorld*  world = dcwCollisionWorldCreate();

	dcwScalar* eX = dcwScalarCreate(1.0f);
	dcwScalar* eY = dcwScalarCreate(2.0f);
	dcwScalar* eZ = dcwScalarCreate(4.0f);

	dcwVector3*         boxSize = dcwVector3Create(eX,eY,eZ);
	dcwBoxShape*        boxShape = dcwBoxShapeCreate(boxSize);

	dcwCollisionObject* object = dcwCollisionObjectCreate();
	dcwCollisionObject* object2 = dcwCollisionObjectCreate();

	dcwCollisionObjectSetCollisionBoxShape(object, boxShape);
	dcwCollisionObjectSetCollisionBoxShape(object2, boxShape);

	dcwCollisionWorldAddCollisionObject(world,object);

	unitAssertEqualInt("Add obbject to Collision World",
	                  dcwCollisionWorldGetNumCollisionObjects(world),1);

	dcwCollisionWorldAddCollisionObject(world,object2);

	unitAssertEqualInt("Add obbject2 to Collision World",
	                  dcwCollisionWorldGetNumCollisionObjects(world), 2);


	unitPrintComment("Perform Discrete Collision Detection on World");

	dcwCollisionWorldPerformDiscreteCollisionDetection(world);

	return;
}

void testColWorldTwoBoxCollision() {
	unitPrintComment("Perform Discrete Collision Detection with two box shapes");

	unitPrintComment("PDCD: Collision World Add Objects");
	dcwCollisionWorld*  world = dcwCollisionWorldCreate();

	dcwScalar* eX = dcwScalarCreate(2.0f);
	dcwScalar* eY = dcwScalarCreate(2.0f);
	dcwScalar* eZ = dcwScalarCreate(2.0f);

	dcwVector3*         boxSize = dcwVector3Create(eX,eY,eZ);
	dcwBoxShape*        boxShape = dcwBoxShapeCreate(boxSize);

	dcwCollisionObject* object = dcwCollisionObjectCreate();
	dcwCollisionObject* object2 = dcwCollisionObjectCreate();

	dcwCollisionObjectSetCollisionBoxShape(object, boxShape);
	dcwCollisionObjectSetCollisionBoxShape(object2, boxShape);

	dcwCollisionWorldAddCollisionObject(world,object);

	unitAssertEqualInt("Add obbject to Collision World",
	                  dcwCollisionWorldGetNumCollisionObjects(world),1);

	dcwCollisionWorldAddCollisionObject(world,object2);

	unitAssertEqualInt("Add obbject2 to Collision World",
	                  dcwCollisionWorldGetNumCollisionObjects(world), 2);

	unitPrintComment("Translating Boxes");
	dcwScalar *minusOne, *one, *zero; 

	minusOne = dcwScalarCreate(-1.0f);
	zero = dcwScalarCreate(0.0f);
	one = dcwScalarCreate(1.0f);

	dcwVector3   *transVec1  = dcwVector3Create(minusOne,zero,zero);
	dcwTransform *transform1 = dcwTransformCreate();
	dcwTransformSetOrigin(transform1,transVec1);

	dcwVector3 *transVec2    = dcwVector3Create(one,zero,zero);
	dcwTransform *transform2 = dcwTransformCreate();
	dcwTransformSetOrigin(transform2,transVec2);

	dcwCollisionObjectForceActivationState(object, DISABLE_DEACTIVATION);
	dcwCollisionObjectSetWorldTransform(object, transform1);

	dcwCollisionObjectForceActivationState(object2, DISABLE_DEACTIVATION);
	dcwCollisionObjectSetWorldTransform(object2, transform2);

	unitPrintComment("Perform Discrete Collision Detection on World");

	dcwCollisionWorldPerformDiscreteCollisionDetection(world);

	dcwSortedOverlappingPairCache* pairCache = dcwSortedOverlappingPairCacheCreate();

	int numManifolds = dcwCollisionWorldGetDispatcher(world)->getNumManifolds();
	for (int i=0;i<numManifolds;i++) {
		dcwPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		dcwCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
		dcwCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());
		int numContacts = contactManifold->getNumContacts();
		for (int j=0;j<numContacts;j++) {
			dcwManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance()<0.f) {
				const dcwVector3& ptA = pt.getPositionWorldOnA();
				const dcwVector3& ptB = pt.getPositionWorldOnB();
				const dcwVector3& normalOnB = pt.m_normalWorldOnB;
			}
		}
	}
	unitAssertFail("Two Box Collision");
	return;

}
