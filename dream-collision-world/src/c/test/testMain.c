#include "testScalar.h"
#include "testVector3.h"
#include "testBoxShape.h"
#include "testCollisionObject.h"
#include "testSortedOverlappingPairCache.h"
#include "testCollisionWorld.h"
#include "../../../../unit/src/unit.h"

int main(int argc, char** argv) {
	unitModuleHeading("Dream Collision World");
	testScalar();
	testVector3();
	testBoxShape();
	testCollisionObject();
	testSortedOverlappingPairCache();
	testCollisionWorld();
	return 0;
}
