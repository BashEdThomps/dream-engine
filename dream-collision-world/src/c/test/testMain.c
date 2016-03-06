#include "testVector3.h"
#include "testBoxShape.h"
#include "testCollisionObject.h"
#include "testCollisionWorld.h"

int main(int argc, char** argv) {
	testVector3();
	testBoxShape();
	testCollisionObject();
	testCollisionWorld();
	return 0;
}
