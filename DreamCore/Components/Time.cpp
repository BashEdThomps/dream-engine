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

}
