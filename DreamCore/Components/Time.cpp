/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "Time.h"

#include <iostream>

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
