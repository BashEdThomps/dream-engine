#include "../../../../unit/src/unit.h"

#include "../../cpp/CWrapper/dcwCollisionWorld.h"
#include "../../cpp/CWrapper/dcwBoxShape.h"
#include "../../cpp/CWrapper/dcwVector3.h"
#include "../../cpp/CWrapper/dcwTransform.h"
#include "../../cpp/CWrapper/dcwSortedOverlappingPairCache.h"
#include "../../cpp/CWrapper/dcwManifoldPoint.h"
#include "../../cpp/CWrapper/dcwAxisSweep3.h"
#include "../../cpp/CWrapper/dcwScalar.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "testCollisionWorld.h"

void testCollisionWorld(void) {
	unitTestHeading("Testing CollisionWord");
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
	collisionWorld = _createCollisionWorld();
	unitAssertNotNull("Checking collision world is not NULL", collisionWorld);
	return;
}

void testColWorldAddObject(void) {
	unitPrintComment("Collision World Add Object");
	dcwCollisionWorld *world =  _createCollisionWorld();

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
	dcwCollisionWorld*  world = _createCollisionWorld();

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
	dcwScalar* two                = dcwScalarCreate(2.0f);
	dcwVector3*         boxSize1  = dcwVector3Create(two,two,two);
	dcwBoxShape*        boxShape1 = dcwBoxShapeCreate(boxSize1);
	dcwCollisionObject* object1   = dcwCollisionObjectCreate();
	dcwCollisionWorld *world      = _createCollisionWorld();

	dcwCollisionObjectSetCollisionBoxShape (object1, boxShape1);
	dcwCollisionWorldAddCollisionObject    (world,object1);
	unitAssertEqualInt("Add obbject to Collision World", dcwCollisionWorldGetNumCollisionObjects(world),1);

	dcwVector3*         boxSize2  = dcwVector3Create  (two,two,two);
	dcwBoxShape*        boxShape2 = dcwBoxShapeCreate (boxSize2);
	dcwCollisionObject* object2 = dcwCollisionObjectCreate();
	dcwCollisionObjectSetCollisionBoxShape (object2, boxShape2);
	dcwCollisionWorldAddCollisionObject    (world,object2);
	unitAssertEqualInt("Add obbject2 to Collision World", dcwCollisionWorldGetNumCollisionObjects(world), 2);

	unitPrintComment("Perform Discrete Collision Detection on World");
	dcwCollisionWorldPerformDiscreteCollisionDetection(world);

	unitPrintComment("Done PDCD");
	return;
}

void testColWorldTwoBoxCollision() {
	unitPrintComment("Perform Discrete Collision Detection with two box shapes");
	dcwCollisionWorld*  world = _createCollisionWorld();

	dcwScalar* four = dcwScalarCreate(2.5f);

	dcwVector3*         boxSize1  = dcwVector3Create(four,four,four);
	dcwBoxShape*        boxShape1 = dcwBoxShapeCreate(boxSize1);
	dcwCollisionObject* object1   = dcwCollisionObjectCreate();
	dcwCollisionObjectSetCollisionBoxShape(object1, boxShape1);

	dcwVector3*         boxSize2  = dcwVector3Create(four,four,four);
	dcwBoxShape*        boxShape2 = dcwBoxShapeCreate(boxSize2);
	dcwCollisionObject* object2   = dcwCollisionObjectCreate();
	dcwCollisionObjectSetCollisionBoxShape(object2, boxShape2);

	dcwCollisionWorldAddCollisionObject(world,object1);
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
	dcwTransformSetIdentity(transform1);
	dcwTransformSetOrigin(transform1,transVec1);

	dcwVector3 *transVec2    = dcwVector3Create(one,zero,zero);
	dcwTransform *transform2 = dcwTransformCreate();
	dcwTransformSetIdentity(transform2);
	dcwTransformSetOrigin(transform2,transVec2);

	dcwCollisionObjectForceActivationState(object1, DISABLE_DEACTIVATION);
	dcwCollisionObjectForceActivationState(object2, DISABLE_DEACTIVATION);

	dcwCollisionObjectSetWorldTransform(object1, transform1);
	dcwCollisionObjectSetWorldTransform(object2, transform2);

	unitPrintComment("Perform Discrete Collision Detection on World");
	dcwCollisionWorldPerformDiscreteCollisionDetection(world);

	//dcwSortedOverlappingPairCache* pairCache = dcwSortedOverlappingPairCacheCreate();
	dcwCollisionDispatcher* dispatcher = dcwCollisionWorldGetDispatcher(world);

	int numManifolds = dcwCollisionDispatcherGetNumManifolds(dispatcher);
	char buffer[256];
	snprintf(&buffer[0],256,"%d Manifolds",numManifolds);
	unitPrintComment(&buffer[0]);
	unitAssertNotZero("Num Manifolds found",numManifolds);

	int i;
	dcwPersistentManifold *nextManifold = 0;
	for (i=0; i<numManifolds; i++) {
		nextManifold = dcwCollisionDispatcherGetManifoldByIndexInternal(dispatcher, i);

		if (nextManifold == 0) {
			unitPrintComment("Found Null Manifold in Dispatcher...");
			continue;
		}

		unitPrintComment("Found a Manifold in Dispatcher...");

		const dcwCollisionObject* objA = dcwPersistentManifoldGetBody0(nextManifold);
		const dcwCollisionObject* objB = dcwPersistentManifoldGetBody1(nextManifold);

		int numContacts = dcwPersistentManifoldGetNumContacts(nextManifold);
		snprintf(&buffer[0],256,"%d Contacts found",numManifolds);
		unitPrintComment(&buffer[0]);
		unitAssertNotZero("Number of contacts",numContacts);

		dcwTransform* objAWorldTrans    = dcwCollisionObjectGetWorldTransform ((dcwCollisionObject*)objA);
		dcwVector3*   objAWorldTransVec = dcwTransformGetOrigin               (objAWorldTrans);

		dcwTransform* objBWorldTrans    = dcwCollisionObjectGetWorldTransform ((dcwCollisionObject*)objB);
		dcwVector3*   objBWorldTransVec = dcwTransformGetOrigin               (objBWorldTrans);

		snprintf(&buffer[0],256,"Contact on objects at:\n\tObjA (%.2f,%.2f,%.2f)\n\tObjB (%.2f,%.2f,%.2f)",
			// ObjA
			dcwScalarGetValue(dcwVector3GetX(objAWorldTransVec)),
			dcwScalarGetValue(dcwVector3GetY(objAWorldTransVec)),
			dcwScalarGetValue(dcwVector3GetZ(objAWorldTransVec)),
			// ObjB
			dcwScalarGetValue(dcwVector3GetX(objBWorldTransVec)),
			dcwScalarGetValue(dcwVector3GetY(objBWorldTransVec)),
			dcwScalarGetValue(dcwVector3GetZ(objBWorldTransVec))
		);

		unitPrintComment(&buffer[0]);

		for (int j=0;j<numContacts;j++) {
			dcwManifoldPoint *pt = dcwPersistentManifoldGetContactPoint(nextManifold,j);
			if (dcwScalarGetValue(dcwManifoldPointGetDistance(pt)) < 0.f) {
				const dcwVector3* ptA       = dcwManifoldPointGetPositionWorldOnA (pt);
				const dcwVector3* ptB       = dcwManifoldPointGetPositionWorldOnB (pt);
				const dcwVector3* normalOnB = dcwManifoldPointGetNormalWorldOnB   (pt);

				snprintf(&buffer[0],256,"Manifold Point At:\n\tptA (%.2f,%.2f,%.2f)\n\tptB (%.2f,%.2f,%.2f)\n\tNormalB (%.2f,%.2f,%.2f)",
					// PtA
					dcwScalarGetValue(dcwVector3GetX((dcwVector3*)ptA)),
					dcwScalarGetValue(dcwVector3GetY((dcwVector3*)ptA)),
					dcwScalarGetValue(dcwVector3GetZ((dcwVector3*)ptA)),
					// PtB
					dcwScalarGetValue(dcwVector3GetX((dcwVector3*)ptB)),
					dcwScalarGetValue(dcwVector3GetY((dcwVector3*)ptB)),
					dcwScalarGetValue(dcwVector3GetZ((dcwVector3*)ptB)),
					// NormalB
					dcwScalarGetValue(dcwVector3GetX((dcwVector3*)normalOnB)),
					dcwScalarGetValue(dcwVector3GetY((dcwVector3*)normalOnB)),
					dcwScalarGetValue(dcwVector3GetZ((dcwVector3*)normalOnB))
				);

		        unitPrintComment(&buffer[0]);
			}
		}
	}
	return;
}

dcwCollisionWorld* _createCollisionWorld(void) {
	dcwScalar* minusOneHundred = dcwScalarCreate(-100.0f);
	dcwScalar* oneHundred      = dcwScalarCreate(100.0f);

	dcwVector3 *aabbMin = dcwVector3Create(minusOneHundred,minusOneHundred,minusOneHundred);
	dcwVector3 *aabbMax = dcwVector3Create(oneHundred,oneHundred,oneHundred);

	dcwAxisSweep3*                    a3PairCache   = dcwAxisSweep3Create(aabbMin, aabbMax);
	dcwDefaultCollisionConfiguration* collisionConf = dcwDefaultCollisionConfigurationCreate();
	dcwCollisionDispatcher*           dispatcher    = dcwCollisionDispatcherCreate(collisionConf);
	return dcwCollisionWorldCreate(dispatcher, a3PairCache, collisionConf);
}
