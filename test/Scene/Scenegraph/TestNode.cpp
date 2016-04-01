#include "TestSceneObject.h"
#include "../SceneObject.h"

namespace DreamScenegraphTest {
	TestSceneObject::TestSceneObject(void){

	}

	TestSceneObject::~TestSceneObject(void){

	}

	void TestSceneObject::run(void) {
		testSceneObject();
	}

	void TestSceneObject::testSceneObject() {
		unitTestHeading("SceneObject");

		dsgSceneObject* root = (dsgSceneObject*)malloc(sizeof(*root));
		dsgSceneObject* child1 = (dsgSceneObject*)malloc(sizeof(*child1));
		dsgSceneObject* child2 = (dsgSceneObject*)malloc(sizeof(*child2));
		dsgSceneObject* child1_1 = (dsgSceneObject*)malloc(sizeof(*child1_1));
		dsgSceneObject* child1_1_1 = (dsgSceneObject*)malloc(sizeof(*child1_1_1));

		dsgSceneObjectInit(root,"Root");
		unitAssertZero("Root node name is Root",strcmp(root->name,"Root"));

		dsgSceneObjectInit(child1,"Child1");
		unitAssertZero("Child1 name is Child1",strcmp(child1->name,"Child1"));

		dsgSceneObjectInit(child2,"Child2");
		unitAssertZero("Child2 name is Child2",strcmp(child2->name,"Child2"));

		dsgSceneObjectInit(child1_1,"Child1_1");
		unitAssertZero("Child1_1 name is Child1_1",strcmp(child1_1->name,"Child1_1"));

		dsgSceneObjectInit(child1_1_1,"Child1_1_1");
		unitAssertZero("Child1_1_1 name is Child1_1_1",strcmp(child1_1_1->name,"Child1_1_1"));

		unitTestFooter("SceneObject");
	}
}
