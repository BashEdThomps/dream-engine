#include "GLFWTime.h"

namespace Dream   {
namespace Plugins {
namespace Video   {
namespace OpenGL  {
	
	GLFWTime *GLFWTime::sInstance = NULL;

	GLFWTime::GLFWTime()  {
		mCurrentTime = 0.0;
		mLastTime    = 0.0;
		mTimeDelta   = 0.0;
	}
	
	GLFWTime::~GLFWTime() {}
	GLFWTime* GLFWTime::getInstance() {
		if (!sInstance) {
			sInstance = new GLFWTime;
		}
		return sInstance;
	}

	void GLFWTime::update() {
		mLastTime    = mCurrentTime;
		mCurrentTime = glfwGetTime();
		mTimeDelta   = mCurrentTime - mLastTime;
	}
	
	double GLFWTime::getCurrentTime() { return mCurrentTime; }
	double GLFWTime::getLastTime()    { return mLastTime;    }
	double GLFWTime::getTimeDelta()   { return mTimeDelta;   }
	
}
}
}
}
