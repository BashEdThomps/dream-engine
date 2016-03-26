#include "TestNode.h"
#include "TestScenegraph.h"
#include "TestCamera.h"

int main(void) {
	int result = 0;

	TestCamera testCamera;
	testCamera.run();
	result += testCamera.getResult();

	TestNode testNode;
	testNode.run();
	resut += testNode.getResult();

	TestGraph testScenegraph;
	testScenegraph.run();
	result += testScenegraph.getResult();

	return result;
}
