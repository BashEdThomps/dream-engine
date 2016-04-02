/*
* TestScenegraph.cpp
*/

#include <iostream>

#include "TestScenegraph.h"
#include "../../src/Scenegraph/Scenegraph.h"
#include "../../src/Scenegraph/SceneObject.h"

namespace Dream {
	namespace Scenegraph {
		namespace Test {
			TestScenegraph::TestScenegraph() : Dream::Unit::Unit("Dream::Scene::Scenegraph::"){

			}

			TestScenegraph::~TestScenegraph() {

			}

			void TestScenegraph::run() {
				header();
				testScenegraph();
			}

			void TestScenegraph::testScenegraph() {
				testHeading("Scenegraph");
				// Create Scenegraph
				Dream::Scenegraph::Scenegraph* graph = new Dream::Scenegraph::Scenegraph("MySceneGraph");

				// Create SceneObjects
				printComment("Creating SceneObjects");
				Dream::Scenegraph::SceneObject *n1, *n2, *n3, *n4, *n5;
				n1 = new SceneObject("SceneObject1");
				n2 = new SceneObject("SceneObject2");
				n3 = new SceneObject("SceneObject3");
				n4 = new SceneObject("SceneObject4");
				n5 = new SceneObject("SceneObject5");

				// Establish Relationships
				printComment("Setting Relationships");
				graph->setRootSceneObject(n1);
				ScenegraphSceneObjectSetParent(n1,n2);
				ScenegraphSceneObjectSetParent(n1,n3);
				ScenegraphSceneObjectSetParent(n3,n4);
				ScenegraphSceneObjectSetParent(n3,n5);
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
				assertNotZero("N1 has 2 children", graph->countChildrenOfSceneObject(n1) == 2);
				assertNotZero("N2 has 0 children", graph->countChildrenOfSceneObject(n2) == 0);
				assertNotZero("N2 has 2 children", graph->countChildrenOfSceneObject(n3) == 2);
				assertNotZero("N4 has 0 children", graph->countChildrenOfSceneObject(n4) == 0);
				assertNotZero("N5 has 0 children", graph->countChildrenOfSceneObject(n5) == 0);
				graph->printGraph();

				// Test name lookup
				printComment("Testing SceneObject Lookup By Name");
				SceneObject* result = graph->getSceneObjectByName(n4->name);
				assertNotZero("Found n4 by name",result == n4);
				// Test Traverse Path
				printComment("Testing SceneObject Path Traversal");
				int traverseResult = graph->traversePath(n4->path,&TestScenegraph::traverseFunction,NULL);
				assertNotZero("Traverse Path Result",traverseResult);
				testFooter("Graph");
				return;
			}
		};
	}
}
