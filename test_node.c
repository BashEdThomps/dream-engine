#include <unit.h>
#include "test_node.h"
#include "node.h"

void testNode() {
	unitSubheading("Node");
	node_t* root = (node_t*)malloc(sizeof(*root));
	node_t* child1 = (node_t*)malloc(sizeof(*child1));
	node_t* child2 = (node_t*)malloc(sizeof(*child2));
	node_t* child1_1 = (node_t*)malloc(sizeof(*child1_1));
	node_t* child1_1_1 = (node_t*)malloc(sizeof(*child1_1_1));

	dsNodeAddChild(root,child1);	
	dsNodeAddChild(root,child2);
	dsNodeAddChild(child1,child1_1);
	dsNodeAddChild(child1_1,child1_1_1);
	dsNodePrintGraph(root);
		unitFooter();
}
