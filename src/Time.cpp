#include "Time.h"

namespace Dream {

  Time::Time()  {
    mCurrentTime = chrono::high_resolution_clock::now();
    mLastTime = chrono::high_resolution_clock::now();
  }

  Time::~Time() {}

  void Time::update() {
    mLastTime    = mCurrentTime;
    mCurrentTime = chrono::high_resolution_clock::now();
    mTimeDelta   = mCurrentTime-mLastTime;
    //show();
    return;
  }

  void Time::show() {
    if (DEBUG) {
      cout << "Time: " << endl
           << "\tCurrent Time: " << getCurrentTime() << endl
           << "\t   Last Time: " << getLastTime() << endl
           << "\t  Time Delta: " << getTimeDelta() << endl;
    }
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

} // End of Dream
