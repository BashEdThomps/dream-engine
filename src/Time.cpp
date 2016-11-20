#include "Time.h"

namespace Dream {

  Time::Time()  {}
  Time::~Time() {}

  void Time::update() {
    mLastTime    = mCurrentTime;
    mCurrentTime = std::chrono::high_resolution_clock::now();
    mTimeDelta   = mCurrentTime-mLastTime;
    #ifdef VERBOSE
    std::cout << "Time: " << std::endl
              << "\tCurrent Time: " << getCurrentTime() << std::endl
              << "\t   Last Time: " << getLastTime() << std::endl
              << "\t  Time Delta: " << getTimeDelta() << std::endl;
    #endif
  }

  double Time::getCurrentTime() {
    return mCurrentTime.time_since_epoch().count();
  }

  double Time::getLastTime() {
    return mLastTime.time_since_epoch().count();
  }

  double Time::getTimeDelta() {
    return mTimeDelta.count();
 }

}
