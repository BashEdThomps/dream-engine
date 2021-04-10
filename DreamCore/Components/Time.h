#pragma once

#include <chrono>

using std::chrono::time_point;
using std::chrono::steady_clock;
using std::chrono::duration;
using std::chrono::seconds;

namespace octronic::dream
{
    /**
     * @brief Manages timinng features for Dream. All times use
     * std::steady_clock and are reported in fractions of a second.
     * e.g. 0.34s, 2.38s etc...
     */
    class Time
    {
    public:
        /**
         * @brief Default Constructor
         */
        Time();

        /**
          @brief Default Destructor
        */
        ~Time();

        /**
         * @return Gets the time that the current frame begain
         */
        long getCurrentFrameTime();

        /**
         * @return Get the time that the last frame began
         */
        long getLastFrameTime();

        /**
         * @return Get the delta between this frame and the last
         */
        long getFrameTimeDelta();

        /**
         * @brief Scale the value passed by the amount of time that has passed
         * between frames. Used to get frame-independant timings
         */
        double perSecond(double value);

        /**
         * @brief Update the current and last frame time values.
         */
        void updateFrameTime();

        /**
         * @brief Print Debug information about the Time object's state.
         */
        void show();

        /**
         * @return Get the time at the point of function call. Different from
         * @see getCurrentFrameTime
         */
        long getAbsoluteTime();

        const static int DELTA_MAX;

    private:
        /**
         * @brief Current time
         */
        long mCurrentFrameTime;

        /**
         * @brief Time of last frame
         */
        long mLastFrameTime;
    };
}
