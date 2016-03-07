#include "testScalar.h"
#include "testVector3.h"
#include "testBoxShape.h"
#include "testCollisionObject.h"
#include "testSortedOverlappingPairCache.h"
#include "testCollisionWorld.h"

int main(int argc, char** argv) {
	testScalar();
	testVector3();
	testBoxShape();
	testCollisionObject();
	testSortedOverlappingPairCache();
	testCollisionWorld();
	return 0;
}
