#include "TestFrame.h"
#include "TestFrameDelta.h"
#include "TestKeyFrame.h"
#include "TestKeyFrameAnimation.h"
#include "../../../unit/src/unit.h"

int main(void) {
	unitModuleHeading("DreamAnimation");
	testFrame();
	testFrameDelta();
	testKeyFrame();
	testKeyFrameAnimation();
}
