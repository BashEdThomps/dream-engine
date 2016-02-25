#include <unit.h>
#include "test_node.h"
#include "test_graph.h"

int main(int argc, char** argv) {
	unitSetModuleName("DreamSceneGraph");
	unitModuleHeading();
	testNode();
	testGraph();
	unitModuleFooter();
	return 0;
}
