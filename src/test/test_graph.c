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
	fprintf(stdout, "Creating Nodes\n");
	node_t *n1, *n2, *n3, *n4, *n5;
	n1 = dsgGraphCreateNodeWithName(graph,"Node1");
	n2 = dsgGraphCreateNodeWithName(graph,"Node2");
	n3 = dsgGraphCreateNodeWithName(graph,"Node3");
	n4 = dsgGraphCreateNodeWithName(graph,"Node4");
	n5 = dsgGraphCreateNodeWithName(graph,"Node5");

	// Establish Relationships
	fprintf(stdout, "Setting Relationships\n");
	dsgGraphSetRootNode(graph, n1);
	dsgNodeSetParent(n1,n2);
	dsgNodeSetParent(n1,n3);
	dsgNodeSetParent(n3,n4);
	dsgNodeSetParent(n3,n5);

	fprintf(stdout,"Updating Graph\n");
	dsgGraphUpdateAll(graph);

	// Test Parent Relationships
	fprintf(stdout, "Testing Parent Relationships\n");
	unitAssertNotZero("N1 parent of N2",dsgNodeIsParentOf(n1,n2));
	unitAssertNotZero( "N1 parent of N3",dsgNodeIsParentOf(n1,n3));
	unitAssertNotZero("N3 parent of N4",dsgNodeIsParentOf(n3,n4));
	unitAssertNotZero("N3 parent of N5",dsgNodeIsParentOf(n3,n5));
	unitAssertZero("N5 NOT parent of N3",dsgNodeIsParentOf(n5,n3));

	// Test Child Relationships
	fprintf(stdout, "Testing Child Relationships\n");
	unitAssertNotZero("N2 child of N1",dsgNodeIsParentOf(n1,n2));
	unitAssertNotZero( "N3 child of N1",dsgNodeIsParentOf(n1,n3));
	unitAssertNotZero("N4 child of N3",dsgNodeIsParentOf(n3,n4));
	unitAssertNotZero("N5 child of N3",dsgNodeIsParentOf(n3,n5));
	unitAssertNotZero("N5 child of N1",dsgNodeIsParentOf(n1,n5));
	unitAssertZero("N3 NOT child of N4",dsgNodeIsParentOf(n4,n3));

	// Test counting children
	unitAssertNotZero("N1 has 4 children",dsgGraphCountChildrenOfNode(graph, n1) == 4);
	unitAssertNotZero("N2 has 0 children",dsgGraphCountChildrenOfNode(graph, n2) == 0);
	unitAssertNotZero("N2 has 2 children",dsgGraphCountChildrenOfNode(graph, n3) == 2);
	unitAssertNotZero("N4 has 0 children",dsgGraphCountChildrenOfNode(graph, n4) == 0);
	unitAssertNotZero("N5 has 0 children",dsgGraphCountChildrenOfNode(graph, n5) == 0);

	dsgGraphPrintGraph(graph);

	unitTestFooter("Graph");
	return;
}
