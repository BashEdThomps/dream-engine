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


#pragma once

#include <chrono>
#include "../Common/DreamObject.h"

using std::chrono::time_point;
using std::chrono::high_resolution_clock;
using std::chrono::duration;

namespace Dream
{
    /**
     * @brief Manages time/temperal features for Dream. All times use
     * std::high_resolution_clock and are reported in fractions of a second.
     * e.g. 0.34s, 2.38s etc...
     */
    class Time : public DreamObject
    {
    private:
        /**
         * @brief Current time
         */
        time_point<high_resolution_clock> mCurrentTime;
        /**
         * @brief Time of last frame
         */
        time_point<high_resolution_clock> mLastTime;
        /**
         * @brief Delta of Current Time and Last Frame Time
         */
        duration<double> mTimeDelta;
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
        double getCurrentFrameTime();

        /**
         * @return Get the time that the last frame began
         */
        double getLastFrameTime();

        /**
         * @return Get the delta between this frame and the last
         */
        double getFrameTimeDelta();

        /**
         * @brief Scale the value passed by the amount of time that has passed
         * between frames. Used to get frame-independant timings
         */
        double scaleValueByFrameTime(double value);

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
        double now();

        /**
         * @return Get the time at the point of function call (in long long format)
         */
        long long nowLL();
    };

} // End of Dream
