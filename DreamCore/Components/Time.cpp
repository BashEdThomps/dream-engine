#include "Time.h"

#include <iostream>
#include "../Common/Constants.h"

using std::cout;
using std::endl;


namespace Dream
{
    Time::Time
    ()
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
            cout << "Time: Update Called" << endl;
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
            cout << "Time: " << endl
                 << "\tCurrent Time: " << getCurrentTime() << endl
                 << "\t   Last Time: " << getLastTime() << endl
                 << "\t  Time Delta: " << getTimeDelta() << endl;
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
