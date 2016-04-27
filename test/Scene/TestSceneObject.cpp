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
				assertTrue("Can store name",s.getName().compare(name) == 0);
			}

			void TestSceneObject::testRelationships() {
				assertInconclusive("Test Relationships (never ends well)");
			}
		}
	}
}
