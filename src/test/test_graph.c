#include <unit.h>
#include <stdio.h>

#include "../graph.h"
#include "../node.h"
#include "test_graph.h"

void testGraph() {
	unitTestHeading("Graph");

	// Create Graph
	graph_t* graph = dsgGraphInitWithName("MySceneGraph");

	// Create Nodes	
	unitPrintComment("Creating Nodes");
	node_t *n1, *n2, *n3, *n4, *n5;
	n1 = dsgGraphCreateNodeWithName(graph,"Node1");
	n2 = dsgGraphCreateNodeWithName(graph,"Node2");
	n3 = dsgGraphCreateNodeWithName(graph,"Node3");
	n4 = dsgGraphCreateNodeWithName(graph,"Node4");
	n5 = dsgGraphCreateNodeWithName(graph,"Node5");

	// Establish Relationships
	unitPrintComment("Setting Relationships");
	dsgGraphSetRootNode(graph, n1);
	dsgGraphNodeSetParent(graph,n1,n2);
	dsgGraphNodeSetParent(graph,n1,n3);
	dsgGraphNodeSetParent(graph,n3,n4);
	dsgGraphNodeSetParent(graph,n3,n5);

	unitPrintComment("Updating Graph");
	dsgGraphUpdateAll(graph);

	// Test Parent Relationships
	unitPrintComment("Testing Parent Relationships");
	unitAssertNotZero("N1 parent of N2",dsgGraphNodeIsParentOf(graph,n1,n2));
	unitAssertNotZero("N1 parent of N3",dsgGraphNodeIsParentOf(graph,n1,n3));
	unitAssertNotZero("N3 parent of N4",dsgGraphNodeIsParentOf(graph,n3,n4));
	unitAssertNotZero("N3 parent of N5",dsgGraphNodeIsParentOf(graph,n3,n5));
	unitAssertZero("N5 NOT parent of N3",dsgGraphNodeIsParentOf(graph,n5,n3));

	// Test Child Relationships
	unitPrintComment("Testing Child Relationships");
	unitAssertNotZero("N2 child of N1",dsgGraphNodeIsChildOf(graph,n1,n2));
	unitAssertNotZero("N3 child of N1",dsgGraphNodeIsChildOf(graph,n1,n3));
	unitAssertNotZero("N4 child of N3",dsgGraphNodeIsChildOf(graph,n3,n4));
	unitAssertNotZero("N5 child of N3",dsgGraphNodeIsChildOf(graph,n3,n5));
	unitAssertNotZero("N5 child of N1",dsgGraphNodeIsChildOf(graph,n1,n5));
	unitAssertZero("N3 NOT child of N4",dsgGraphNodeIsChildOf(graph,n4,n3));

	// Test Immediate Parent Relationships
	unitPrintComment("Testing immediate parent Relationships");
	unitAssertNotZero("N1 immediate parent of N2",dsgGraphNodeIsImmediateParentOf(graph,n1,n2));
	unitAssertNotZero("N1 immediate parent of N3",dsgGraphNodeIsImmediateParentOf(graph,n1,n3));
	unitAssertNotZero("N3 immediate parent of N4",dsgGraphNodeIsImmediateParentOf(graph,n3,n4));
	unitAssertNotZero("N3 immediate parent of N5",dsgGraphNodeIsImmediateParentOf(graph,n3,n5));
	unitAssertZero("N1 NOT immediate parent of N5",dsgGraphNodeIsImmediateParentOf(graph,n1,n5));
	unitAssertZero("N4 NOT immediate parent of N3",dsgGraphNodeIsImmediateParentOf(graph,n4,n3));

	// Test counting children
	unitPrintComment("Testing counting children");
	unitAssertNotZero("N1 has 4 children",dsgGraphCountChildrenOfNode(graph, n1) == 4);
	unitAssertNotZero("N2 has 0 children",dsgGraphCountChildrenOfNode(graph, n2) == 0);
	unitAssertNotZero("N2 has 2 children",dsgGraphCountChildrenOfNode(graph, n3) == 2);
	unitAssertNotZero("N4 has 0 children",dsgGraphCountChildrenOfNode(graph, n4) == 0);
	unitAssertNotZero("N5 has 0 children",dsgGraphCountChildrenOfNode(graph, n5) == 0);

	// Test counting immediate children
	unitPrintComment("Testing counting immediate children");
	unitAssertNotZero("N1 has 2 immediate children",
			                      dsgGraphCountImmediateChildrenOfNode(graph, n1) == 2);
	unitAssertNotZero("N2 has 0 immediate children",
			                      dsgGraphCountImmediateChildrenOfNode(graph, n2) == 0);
	unitAssertNotZero("N3 has 2 immediate children",
			                      dsgGraphCountImmediateChildrenOfNode(graph, n3) == 2);
	unitAssertNotZero("N4 has 0 immediate children",
			                      dsgGraphCountImmediateChildrenOfNode(graph, n4) == 0);
	unitAssertNotZero("N5 has 0 immediate children",
			                      dsgGraphCountImmediateChildrenOfNode(graph, n5) == 0);

	dsgGraphPrintGraph(graph);
	unitTestFooter("Graph");
	return;
}
