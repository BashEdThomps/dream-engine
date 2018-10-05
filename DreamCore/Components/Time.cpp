#include "Time.h"

#include <iostream>
#include "../Common/Constants.h"

using std::cout;
using std::endl;


namespace Dream
{
    Time::Time
    ()
        : DreamObject ("Time")
    {
        mCurrentTime = high_resolution_clock::now();
        mLastTime = high_resolution_clock::now();
    }

    Time::~Time
    ()
    {

    }

    void
    Time::updateFrameTime
    ()
    {
        auto log = getLog();
        log->debug( "Time: Update Called" );
        mLastTime    = mCurrentTime;
        mCurrentTime = high_resolution_clock::now();
        mTimeDelta   = mCurrentTime-mLastTime;
        return;
    }

    void
    Time::show
    ()
    {
        auto log = getLog();
        log->debug
        (
           "Time: "
           "\tCurrent Time: {}"
           "\t   Last Time: {}"
           "\t  Time Delta: {}" ,
           getCurrentFrameTime(),
           getLastFrameTime(),
           getFrameTimeDelta()
        );
    }

    double
    Time::getCurrentFrameTime
    ()
    {
        return mCurrentTime.time_since_epoch().count();
    }

    double
    Time::getLastFrameTime
    ()
    {
        return mLastTime.time_since_epoch().count();
    }

    double
    Time::getFrameTimeDelta
    ()
    {
        return mTimeDelta.count();
    }

    double
    Time::scaleValueByFrameTime
    (double value)
    {
        return value*getFrameTimeDelta();
    }

    double
    Time::now()
    {
        return high_resolution_clock::now().time_since_epoch().count();
    }

    long long
    Time::nowLL()
    {
        return high_resolution_clock::now().time_since_epoch().count();
    }

} // End of Dream
