#ifndef GLFWTime_hpp
#define GLFWTime_hpp

#include <GLFW/glfw3.h>

namespace Dream    {
namespace Plugins  {
namespace Video    {
namespace OpenGL   {
	// This class is a singleton
	class GLFWTime {
	private:
  	double mCurrentTime;
  	double mLastTime;
  	double mTimeDelta;
		static GLFWTime *sInstance;
		GLFWTime();
		~GLFWTime();
	public:
		static GLFWTime *getInstance();
  	double getCurrentTime();
  	double getLastTime();
  	double getTimeDelta();
  	void   update();
	};
}
}
}
}

#endif /* GLFWTime_h */
