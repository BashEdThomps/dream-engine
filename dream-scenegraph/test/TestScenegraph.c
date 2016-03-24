/*
* test_graph.c
*/
#include "../../../unit/src/unit.h"
#include <stdio.h>

#include "../dsgScenegraph.h"
#include "../dsgNode.h"
#include "testScenegraph.h"

void traverseFunction(dsgNode* node, void* arg) {
	fprintf(stdout,"Traverse function addressing %s\n",node->name);
	return;
}

void testGraph() {
	unitTestHeading("Graph");
	// Create Graph
	dsgScenegraph* graph = dsgScenegraphInitWithName("MySceneGraph");
	// Create Nodes
	unitPrintComment("Creating Nodes");
	dsgNode *n1, *n2, *n3, *n4, *n5;
	n1 = dsgScenegraphCreateNodeWithName(graph,"Node1");
	n2 = dsgScenegraphCreateNodeWithName(graph,"Node2");
	n3 = dsgScenegraphCreateNodeWithName(graph,"Node3");
	n4 = dsgScenegraphCreateNodeWithName(graph,"Node4");
	n5 = dsgScenegraphCreateNodeWithName(graph,"Node5");
	// Establish Relationships
	unitPrintComment("Setting Relationships");
	dsgScenegraphSetRootNode(graph, n1);
	dsgScenegraphNodeSetParent(graph,n1,n2);
	dsgScenegraphNodeSetParent(graph,n1,n3);
	dsgScenegraphNodeSetParent(graph,n3,n4);
	dsgScenegraphNodeSetParent(graph,n3,n5);
	unitPrintComment("Updating Graph");
	dsgScenegraphUpdatePaths(graph);
	// Test Parent Relationships
	unitPrintComment("Testing Parent Relationships");
	unitAssertNotZero("N1 parent of N2",dsgScenegraphNodeIsParentOf(graph,n1,n2));
	unitAssertNotZero("N1 parent of N3",dsgScenegraphNodeIsParentOf(graph,n1,n3));
	unitAssertNotZero("N3 parent of N4",dsgScenegraphNodeIsParentOf(graph,n3,n4));
	unitAssertNotZero("N3 parent of N5",dsgScenegraphNodeIsParentOf(graph,n3,n5));
	unitAssertZero("N5 NOT parent of N3",dsgScenegraphNodeIsParentOf(graph,n5,n3));
	// Test Child Relationships
	unitPrintComment("Testing Child Relationships");
	unitAssertNotZero("N2 child of N1",dsgScenegraphNodeIsChildOf(graph,n1,n2));
	unitAssertNotZero("N3 child of N1",dsgScenegraphNodeIsChildOf(graph,n1,n3));
	unitAssertNotZero("N4 child of N3",dsgScenegraphNodeIsChildOf(graph,n3,n4));
	unitAssertNotZero("N5 child of N3",dsgScenegraphNodeIsChildOf(graph,n3,n5));
	unitAssertZero("N5 child of N1 (too distant)",dsgScenegraphNodeIsChildOf(graph,n1,n5));
	unitAssertZero("N3 NOT child of N4",dsgScenegraphNodeIsChildOf(graph,n4,n3));
	// Test counting children
	unitPrintComment("Testing counting children");
	unitAssertNotZero("N1 has 2 children",dsgScenegraphCountChildrenOfNode(graph, n1) == 2);
	unitAssertNotZero("N2 has 0 children",dsgScenegraphCountChildrenOfNode(graph, n2) == 0);
	unitAssertNotZero("N2 has 2 children",dsgScenegraphCountChildrenOfNode(graph, n3) == 2);
	unitAssertNotZero("N4 has 0 children",dsgScenegraphCountChildrenOfNode(graph, n4) == 0);
	unitAssertNotZero("N5 has 0 children",dsgScenegraphCountChildrenOfNode(graph, n5) == 0);
	dsgScenegraphPrintGraph(graph);
	// Test name lookup
	unitPrintComment("Testing Node Lookup By Name");
	dsgNode* result = dsgScenegraphGetNodeByName(graph,n4->name);
	unitAssertNotZero("Found n4 by name",result == n4);
	// Test Traverse Path
	unitPrintComment("Testing Node Path Traversal");
	int traverseResult = dsgScenegraphTraversePath(graph,n4->path,&traverseFunction,NULL);
	unitAssertNotZero("Traverse Path Result",traverseResult);
	unitTestFooter("Graph");
	return;
}
