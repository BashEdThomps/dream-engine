#include "Time.h"

namespace Dream {
namespace Scene {
	
	Time::Time()  {
		mCurrentTime = 0.0;
		mLastTime    = 0.0;
		mTimeDelta   = 0.0;
	}
	
	Time::~Time() {}

	void Time::update() {
		mLastTime    = mCurrentTime;
		mCurrentTime = glfwGetTime();
		mTimeDelta   = mCurrentTime - mLastTime;
	}
	
	double Time::getCurrentTime() {
		return mCurrentTime;
	}
	
	double Time::getLastTime() {
		return mLastTime;
	}
	
	double Time::getTimeDelta() {
		return mTimeDelta;
 }
	
}
}
