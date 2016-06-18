#ifndef Time_hpp
#define Time_hpp

#include <GLFW/glfw3.h>

namespace Dream {
namespace Scene {
	
	// This class is a singleton
	class Time {
	private:
  	double mCurrentTime;
  	double mLastTime;
  	double mTimeDelta;
	public:
		Time();
		~Time();
  	double getCurrentTime();
  	double getLastTime();
  	double getTimeDelta();
  	void   update();
	};
	
}
}

#endif /* Time_h */
