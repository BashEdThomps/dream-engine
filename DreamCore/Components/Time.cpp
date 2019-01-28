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

namespace Dream
{
    Time::Time
    () : LockableObject("Time")
    {
        mCurrentFrameTime = 0;
        mLastFrameTime = 0;
    }

    Time::~Time
    ()
    {

    }

    void
    Time::updateFrameTime
    ()
    {
        #ifdef DREAM_LOG
        getLog()->debug( "Time: Update Called" );
        #endif
        mLastFrameTime = mCurrentFrameTime;
        mCurrentFrameTime = std::chrono::duration_cast<std::chrono::milliseconds>
        (steady_clock::now().time_since_epoch()).count();
        #ifdef DREAM_LOG
        show();
        #endif
        return;
    }

    #ifdef DREAM_LOG
    void
    Time::show
    ()
    {
        getLog()->critical
        (
           "Time: Current: {}, Last: {}, Delta: {}" ,
           getCurrentFrameTime(),
           getLastFrameTime(),
           getFrameTimeDelta()
        );
    }
    #endif

    long
    Time::getCurrentFrameTime
    ()
    {
        return mCurrentFrameTime;
    }

    long
    Time::getLastFrameTime
    ()
    {
        return mLastFrameTime;
    }

    long
    Time::getFrameTimeDelta
    ()
    {
        return mCurrentFrameTime - mLastFrameTime;
    }

    double
    Time::perSecond
    (double value)
    {
        double scalar = getFrameTimeDelta()/1000.0;
        double ret = value*scalar;
        #ifdef DREAM_LOG
        getLog()->trace("Scaled by time {} to {} with {}",value,ret,scalar);
        #endif
        return ret;
    }

    long
    Time::getAbsoluteTime
    ()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>
        (steady_clock::now().time_since_epoch()).count();
    }

    const int Time::DELTA_MAX = 100;

} // End of Dream
