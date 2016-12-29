#ifndef Time_hpp
#define Time_hpp

#include "Constants.h"
#include <iostream>
#include <chrono>
#include <ctime>

namespace Dream {

  using namespace std;

  // This class is a singleton
  class Time {
  private:
    chrono::time_point<chrono::high_resolution_clock> mCurrentTime;
    chrono::time_point<chrono::high_resolution_clock> mLastTime;
    chrono::duration<double> mTimeDelta;
  public:
    Time();
    ~Time();
    double getCurrentTime();
    double getLastTime();
    double getTimeDelta();
    void update();
    void show();
  };

} // End of Dream

#endif /* Time_h */
