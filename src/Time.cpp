#include "Time.h"

namespace Dream {

  Time::Time()  {
    mCurrentTime = std::chrono::high_resolution_clock::now();
    mLastTime = std::chrono::high_resolution_clock::now();
  }

  Time::~Time() {}

  void Time::update() {
    mLastTime    = mCurrentTime;
    mCurrentTime = std::chrono::high_resolution_clock::now();
    mTimeDelta   = mCurrentTime-mLastTime;
    //show();
    return;
  }

  void Time::show() {
    std::cout << "Time: " << std::endl
              << "\tCurrent Time: " << getCurrentTime() << std::endl
              << "\t   Last Time: " << getLastTime() << std::endl
              << "\t  Time Delta: " << getTimeDelta() << std::endl;
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
