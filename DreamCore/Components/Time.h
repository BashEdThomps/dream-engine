#pragma once

#include <chrono>
#include "../Common/ILoggable.h"

using std::chrono::time_point;
using std::chrono::high_resolution_clock;
using std::chrono::duration;

namespace Dream
{
    class Time : public ILoggable
    {
    private:
        time_point<high_resolution_clock> mCurrentTime;
        time_point<high_resolution_clock> mLastTime;
        duration<double> mTimeDelta;
    public:
        Time();
        ~Time();
        double getCurrentTime();
        double getLastTime();
        double getTimeDelta();
        double scaleValue(double value);
        void update();
        void show();
    };

} // End of Dream
