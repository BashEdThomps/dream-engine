#ifndef GLFWTime_hpp
#define GLFWTime_hpp

#include <GLFW/glfw3.h>

namespace Dream    {
namespace Plugins  {
namespace Video    {
namespace OpenGL   {
	
	class GLFWTime {
	private:
  	double mCurrentTime;
  	double mLastTime;
  	double mTimeDelta;
	public:
		GLFWTime();
		~GLFWTime();
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
