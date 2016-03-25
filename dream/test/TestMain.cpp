#include "DreamTest.h"
#include "SceneTest.h"


int main(void) {
	int result = 0;

	DreamTest::DreamTest dreamTest;
	dreamTest.run();
	result += dreamTest.getResult();

	DreamTest::SceneTest sceneTest;
	sceneTest.run();
	result += sceneTest.getResult();

	return result;
}
