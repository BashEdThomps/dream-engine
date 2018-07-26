#include "Time.h"

#include <iostream>
#include "../Common/Constants.h"

using std::cout;
using std::endl;


namespace Dream
{
    Time::Time
    ()
        : ILoggable ("Time")
    {
        mCurrentTime = high_resolution_clock::now();
        mLastTime = high_resolution_clock::now();
    }

    Time::~Time
    ()
    {

    }

    void
    Time::update
    ()
    {
        auto log = getLog();
        log->info( "Time: Update Called" );
        mLastTime    = mCurrentTime;
        mCurrentTime = high_resolution_clock::now();
        mTimeDelta   = mCurrentTime-mLastTime;
        //show();
        return;
    }

    void
    Time::show
    ()
    {
        auto log = getLog();
        log->info
        (
           "Time: "
           "\tCurrent Time: {}"
           "\t   Last Time: {}"
           "\t  Time Delta: {}" ,
           getCurrentTime(),
           getLastTime(),
           getTimeDelta()
        );
    }

    double
    Time::getCurrentTime
    ()
    {
        return mCurrentTime.time_since_epoch().count();
    }

    double
    Time::getLastTime
    ()
    {
        return mLastTime.time_since_epoch().count();
    }

    double
    Time::getTimeDelta
    ()
    {
        return mTimeDelta.count();
    }

    double
    Time::scaleValue
    (double value)
    {
        return value*getTimeDelta();
    }

} // End of Dream
