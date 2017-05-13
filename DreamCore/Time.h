#ifndef Time_hpp
#define Time_hpp

#include "Constants.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include "Lua/ILuaExposable.h"

using namespace std;

namespace Dream
{
  class Time : public ILuaExposable
  {
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
    void exposeLuaApi(lua_State*);
  };

} // End of Dream

#endif /* Time_h */
