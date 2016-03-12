#include "testFrame.h"
#include "testFrameDelta.h"
#include "testKeyFrame.h"
#include "testKeyFrameAnimation.h"
#include "../../../unit/src/unit.h"

int main(void) {
	unitModuleHeading("DreamAnimation");
	testFrame();
	testFrameDelta();
	testKeyFrame();
	testKeyFrameAnimation();
}
