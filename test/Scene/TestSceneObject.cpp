#include "TestSceneObject.h"
#include "../../src/Scene/SceneObject.h"

namespace Dream {
	namespace Scene {
		namespace Test {
			TestSceneObject::TestSceneObject(void) : Dream::Unit::Unit("Dream::Scene::SceneObject"){

			}

			TestSceneObject::~TestSceneObject(void){

			}

			void TestSceneObject::run(void) {
				header();
				testCanHoldName();
				testRelationships();
			}

			void TestSceneObject::testCanHoldName() {
				SceneObject s;
				std::string name = "TestSceneObject";
				s.setName(name);
				assertZero("Can store name",s.getName().compare(name) == 0);
			}

			void TestSceneObject::testRelationships() {
				SceneObject* root = new SceneObject();
				SceneObject* child1 = new SceneObject();
				SceneObject* child2 = new SceneObject();
				SceneObject* child1_1 = new SceneObject();
				SceneObject* child1_1_1 = new SceneObject();

				root->setName("Root");

				child1->setName("Child1");
				child1->setParent(root);

				child2->setName("Child2");
				child2->setParent(root);

				child1_1->setName("Child1_1");
				child1_1->setParent(child1);

				child1_1_1->setName("Child1_1_1");
				child1_1_1->setParent(child1_1);
			}
		}
	}
}
