#include "../../../unit/src/unit.h"
#include "testNode.h"
#include "testScenegraph.h"
#include "testCamera.h"

int main(int argc, char** argv) {
	unitModuleHeading("Dream Scenegraph");
	testNode();
	testGraph();
	testCamera();
	return 0;
}
