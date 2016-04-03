/*
* TestScenegraph.cpp
*/

#include <iostream>

#include "TestScenegraph.h"
#include "../../src/Scene/Scenegraph.h"
#include "../../src/Scene/SceneObject.h"

namespace Dream {
	namespace Scene {
		namespace Test {
			TestScenegraph::TestScenegraph() : Dream::Unit::Unit("Dream::Scene::Scenegraph"){
			}

			TestScenegraph::~TestScenegraph() {
			}

			void TestScenegraph::run() {
				header();
				testCanGetSceneObjectByName();
				testCanEstablishRelationships();
				testCanStoreMultipleSceneObjects();
			}

			void TestScenegraph::testCanStoreMultipleSceneObjects() {
				Dream::Scene::Scenegraph* graph = new Dream::Scene::Scenegraph();
				Dream::Scene::SceneObject *n1, *n2, *n3, *n4, *n5;

				n1 = new SceneObject();
				n1->setName("SceneObject1");
				n2 = new SceneObject();
				n2->setName("SceneObject2");
				n3 = new SceneObject();
				n3->setName("SceneObject3");
				n4 = new SceneObject();
				n4->setName("SceneObject4");
				n5 = new SceneObject();
				n5->setName("SceneObject5");

				graph->setRootSceneObject(n1);
				graph->addSceneObjectTo(n2,n1);
				graph->addSceneObjectTo(n3,n1);
				graph->addSceneObjectTo(n4,n3);
				graph->addSceneObjectTo(n5,n3);

				assertEqual("Scenegraph has 5 SceneObjects",graph->getNumberOfSceneObjects(),5);
			}

			void TestScenegraph::testCanEstablishRelationships() {
				Dream::Scene::Scenegraph* graph = new Dream::Scene::Scenegraph();
				Dream::Scene::SceneObject *n1, *n2, *n3, *n4, *n5;

				n1 = new SceneObject();
				n1->setName("SceneObject1");
				n2 = new SceneObject();
				n2->setName("SceneObject2");
				n3 = new SceneObject();
				n3->setName("SceneObject3");
				n4 = new SceneObject();
				n4->setName("SceneObject4");
				n5 = new SceneObject();
				n5->setName("SceneObject5");

				graph->setRootSceneObject(n1);
				graph->addSceneObjectTo(n2,n1);
				graph->addSceneObjectTo(n3,n1);
				graph->addSceneObjectTo(n4,n3);
				graph->addSceneObjectTo(n5,n3);

				assertTrue("N1 parent of N2", n1->isParentOf(n2));
				assertTrue("N1 parent of N3", n1->isParentOf(n3));
				assertTrue("N3 parent of N4", n3->isParentOf(n4));
				assertTrue("N3 parent of N5", n3->isParentOf(n5));
				assertFalse("N5 NOT parent of N3",n5->isParentOf(n3));

				assertTrue("N2 child of N1",n2->isChildOf(n1));
				assertTrue("N3 child of N1",n3->isChildOf(n1));
				assertTrue("N4 child of N3",n4->isChildOf(n3));
				assertTrue("N5 child of N3",n5->isChildOf(n3));
				assertFalse("N2 child NOT of N3",n2->isChildOf(n3));
				assertFalse("N3 NOT child of N4",n3->isChildOf(n4));

				// Test counting children
				assertTrue("N1 has 2 children", graph->countChildrenOfSceneObject(n1) == 2);
				assertTrue("N2 has 0 children", graph->countChildrenOfSceneObject(n2) == 0);
				assertTrue("N2 has 2 children", graph->countChildrenOfSceneObject(n3) == 2);
				assertTrue("N4 has 0 children", graph->countChildrenOfSceneObject(n4) == 0);
				assertTrue("N5 has 0 children", graph->countChildrenOfSceneObject(n5) == 0);
			}

			void TestScenegraph::testCanGetSceneObjectByName() {
				Dream::Scene::Scenegraph* graph = new Dream::Scene::Scenegraph();
				Dream::Scene::SceneObject *n1, *n2, *n3, *n4, *n5;

				n1 = new SceneObject();
				n1->setName("SceneObject1");
				n2 = new SceneObject();
				n2->setName("SceneObject2");
				n3 = new SceneObject();
				n3->setName("SceneObject3");
				n4 = new SceneObject();
				n4->setName("SceneObject4");
				n5 = new SceneObject();
				n5->setName("SceneObject5");

				graph->setRootSceneObject(n1);
				graph->addSceneObjectTo(n2,n1);
				graph->addSceneObjectTo(n3,n1);
				graph->addSceneObjectTo(n4,n3);
				graph->addSceneObjectTo(n5,n3);

				SceneObject* result = graph->getSceneObjectByName(n4->getName());
				assertTrue("Found n4 by name",result == n4);
				return;
			}
		};
	}
}
