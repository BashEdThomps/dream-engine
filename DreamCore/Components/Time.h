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
        double getCurrentFrameTime();
        double getLastFrameTime();
        double getFrameTimeDelta();
        double scaleValueByFrameTime(double value);
        void updateFrameTime();
        void show();
        double now();
        long long nowLL();
    };

} // End of Dream
