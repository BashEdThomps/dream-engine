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

namespace octronic::dream
{
    Time::Time
    ()
        : mCurrentFrameTime(0),
          mLastFrameTime(0)
    {
    }

    Time::~Time
    ()
    {

    }

    void
    Time::updateFrameTime
    ()
    {
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
        LOG_TRACE(
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
        LOG_TRACE("Time: Scaled by time {} to {} with {}",value,ret,scalar);
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
