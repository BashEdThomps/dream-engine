/*
* TestScenegraph.cpp
*/

#include <iostream>

#include "TestScenegraph.h"
#include "../../src/Scenegraph/Scenegraph.h"
#include "../../src/Scenegraph/Node.h"

namespace DreamScenegraphTest {
	TestScenegraph::TestScenegraph() {

	}

	TestScenegraph::TestScenegraph() {

	}

	void TestScenegraph::traverseFunction(Node* node, void* arg) {
		std::cout << "Traverse function addressing " << node->getName() << std::endl;
		return;
	}

	void TestScenegraph::testGraph() {
		testHeading("Scenegraph");
		// Create Scenegraph
		DreamScenegraph::Scenegraph* graph = new DreamScenegraph::Scenegraph("MySceneGraph");

		// Create Nodes
		printComment("Creating Nodes");
		DreamScenegraph::Node *n1, *n2, *n3, *n4, *n5;
		n1 = new Node("Node1");
		n2 = new Node("Node2");
		n3 = new Node("Node3");
		n4 = new Node("Node4");
		n5 = new Node("Node5");

		// Establish Relationships
		printComment("Setting Relationships");
		graph->setRootNode(n1);
		ScenegraphNodeSetParent(n1,n2);
		ScenegraphNodeSetParent(n1,n3);
		ScenegraphNodeSetParent(n3,n4);
		ScenegraphNodeSetParent(n3,n5);
		printComment("Updating Graph");
		graph->updatePaths();

		// Test Parent Relationships
		printComment("Testing Parent Relationships");
		assertNotZero("N1 parent of N2", graph->nodeIsParentOf(n1,n2));
		assertNotZero("N1 parent of N3", graph->nodeIsParentOf(n1,n3));
		assertNotZero("N3 parent of N4", graph->nodeIsParentOf(n3,n4));
		assertNotZero("N3 parent of N5", graph->nodeIsParentOf(n3,n5));
		assertZero("N5 NOT parent of N3",graph->nodeIsParentOf(n5,n3));

		// Test Child Relationships
		printComment("Testing Child Relationships");
		assertNotZero("N2 child of N1",            graph->nodeIsChildOf(n1,n2));
		assertNotZero("N3 child of N1",            graph->nodeIsChildOf(n1,n3));
		assertNotZero("N4 child of N3",            graph->nodeIsChildOf(n3,n4));
		assertNotZero("N5 child of N3",            graph->nodeIsChildOf(n3,n5));
		assertZero("N5 child of N1 (too distant)", graph->nodeIsChildOf(n1,n5));
		assertZero("N3 NOT child of N4",           graph->nodeIsChildOf(n4,n3));

		// Test counting children
		printComment("Testing counting children");
		assertNotZero("N1 has 2 children", graph->countChildrenOfNode(n1) == 2);
		assertNotZero("N2 has 0 children", graph->countChildrenOfNode(n2) == 0);
		assertNotZero("N2 has 2 children", graph->countChildrenOfNode(n3) == 2);
		assertNotZero("N4 has 0 children", graph->countChildrenOfNode(n4) == 0);
		assertNotZero("N5 has 0 children", graph->countChildrenOfNode(n5) == 0);
		graph->printGraph();

		// Test name lookup
		printComment("Testing Node Lookup By Name");
		Node* result = graph->getNodeByName(n4->name);
		assertNotZero("Found n4 by name",result == n4);
		// Test Traverse Path
		printComment("Testing Node Path Traversal");
		int traverseResult = graph->traversePath(n4->path,&TestScenegraph::traverseFunction,NULL);
		assertNotZero("Traverse Path Result",traverseResult);
		testFooter("Graph");
		return;
	}
};
