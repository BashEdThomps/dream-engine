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

#include "Common/Logger.h"

#include <iostream>

using std::unique_lock;

namespace octronic::dream
{
    Time::Time
    () : LockableObject("Time")
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_DEBUG( "Time: Update Called" );
        mLastFrameTime = mCurrentFrameTime;
        mCurrentFrameTime = std::chrono::duration_cast<std::chrono::milliseconds>
        (steady_clock::now().time_since_epoch()).count();

        // Ignore the huge delta on first start
        if (mLastFrameTime == 0)
        {
            mLastFrameTime = mCurrentFrameTime;
        }
        show();
    }

    void
    Time::show
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE
        (
           "Time: CurrentTime: {}, LastTime: {}, DeltaTime: {}" ,
           getCurrentFrameTime(),
           getLastFrameTime(),
           getFrameTimeDelta()
        );
    }

    long
    Time::getCurrentFrameTime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mCurrentFrameTime;
    }

    long
    Time::getLastFrameTime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mLastFrameTime;
    }

    long
    Time::getFrameTimeDelta
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mCurrentFrameTime - mLastFrameTime;
    }

    double
    Time::perSecond
    (double value)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        double scalar = getFrameTimeDelta()/1000.0;
        double ret = value*scalar;
        LOG_TRACE("Time: Scaled by time {} to {} with {}",value,ret,scalar);
        return ret;
    }

    long
    Time::getAbsoluteTime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return std::chrono::duration_cast<std::chrono::milliseconds>
        (steady_clock::now().time_since_epoch()).count();
    }

    const int Time::DELTA_MAX = 100;

} // End of Dream
