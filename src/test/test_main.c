#include <unit.h>
#include "test_node.h"
#include "test_graph.h"

int main(int argc, char** argv) {
	unitModuleHeading("DreamSceneGraph");
	testNode();
	testGraph();
	unitModuleFooter("DreamSceneGraph");
	return 0;
}
