#include "TestFrame.h"
#include "TestFrameDelta.h"
#include "TestKeyFrame.h"
#include "TestAnimation.h"

using namespace DreamAnimationTest;

int main(void) {
	int result = 0;
	TestFrame *testFrame = new TestFrame();
	testFrame->run();
	result += testFrame->getResult();
	delete testFrame;

	TestFrameDelta *testFrameDelta = new TestFrameDelta();
	testFrameDelta->testComputeFrameDelta();
	result += testFrameDelta->getResult();
	delete testFrameDelta;

	TestKeyFrame *testKeyFrame = new TestKeyFrame();
	result += testKeyFrame->getResult();
	delete testKeyFrame;

	TestAnimation *testAnimation = new TestAnimation();
	result += testAnimation->getResult();
	delete testAnimation;
}
