#include <unit.h>
#include <stdio.h>

#include "../graph.h"
#include "../node.h"
#include "test_graph.h"

void testGraph() {
	unitTestHeading("Graph");

	// Create Graph
	graph_t* graph;
	dsgGraphInitWithName(graph,"MySceneGraph");

	// Create Nodes	
	fprintf(stdout, "Creating Nodes\n");
	node_t *n1, *n2, *n3, *n4, *n5;
	dsgGraphCreateNodeWithName(graph,n1,"Node1");
	dsgGraphCreateNodeWithName(graph,n2,"Node2");
	dsgGraphCreateNodeWithName(graph,n3,"Node3");
	dsgGraphCreateNodeWithName(graph,n4,"Node4");
	dsgGraphCreateNodeWithName(graph,n5,"Node5");

	// Establish Relationships
	fprintf(stdout, "Setting Relationships\n");
	dsgNodeSetParent(n1,n2);
	dsgNodeSetParent(n1,n3);
	dsgNodeSetParent(n3,n4);
	dsgNodeSetParent(n3,n5);

	dsgGraphUpdate(&graph);

	// Test Parent Relationships
	fprintf(stdout, "Testing Parent Relationships\n");
	unitAssertNotZero("N1 parent of N2",dsgNodeIsParentOf(n1,n2) > 0);
	unitAssertNotZero( "N1 parent of N3",dsgNodeIsParentOf(n1,n3) > 0);
	unitAssertNotZero("N3 parent of N4",dsgNodeIsParentOf(n3,n4) > 0);
	unitAssertNotZero("N3 parent of N5",dsgNodeIsParentOf(n5,n3) > 0);

	// Test Child Relationships
	fprintf(stdout, "Testing Child Relationships\n");
	unitAssertNotZero("N2 child of N1",dsgNodeIsParentOf(n1,n2) > 0);
	unitAssertNotZero( "N3 child of N1",dsgNodeIsParentOf(n1,n3) > 0);
	unitAssertNotZero("N4 child of N3",dsgNodeIsParentOf(n3,n4) > 0);
	unitAssertNotZero("N5 child of N3",dsgNodeIsParentOf(n5,n3) > 0);

	unitTestFooter("Graph");
	return;
}
