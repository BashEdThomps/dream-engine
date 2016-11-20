#ifndef Time_hpp
#define Time_hpp

#include <iostream>
#include <chrono>
#include <ctime>

//#define VERBOSE

namespace Dream {

  // This class is a singleton
  class Time {
  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> mCurrentTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> mLastTime;
    std::chrono::duration<double> mTimeDelta;
  public:
    Time();
    ~Time();
    double getCurrentTime();
    double getLastTime();
    double getTimeDelta();
    void   update();
  };

}

#endif /* Time_h */
