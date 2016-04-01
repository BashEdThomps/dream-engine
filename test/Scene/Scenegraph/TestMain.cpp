#include "TestSceneObject.h"
#include "TestScenegraph.h"
#include "TestCamera.h"

int main(void) {
	int result = 0;

	TestCamera testCamera;
	testCamera.run();
	result += testCamera.getResult();

	TestSceneObject testSceneObject;
	testSceneObject.run();
	resut += testSceneObject.getResult();

	TestGraph testScenegraph;
	testScenegraph.run();
	result += testScenegraph.getResult();

	return result;
}
