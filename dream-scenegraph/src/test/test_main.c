#include "../../../unit/src/unit.h"
#include "testNode.h"
#include "testScenegraph.h"
#include "testCamera.h"

int main(int argc, char** argv) {
	unitModuleHeading("DreamSceneGraph");
	testNode();
	testGraph();
	testCamera();
	unitModuleFooter("DreamSceneGraph");
	return 0;
}
