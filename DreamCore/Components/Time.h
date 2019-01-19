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
using std::chrono::steady_clock;
using std::chrono::duration;
using std::chrono::seconds;

namespace Dream
{
    /**
     * @brief Manages timinng features for Dream. All times use
     * std::steady_clock and are reported in fractions of a second.
     * e.g. 0.34s, 2.38s etc...
     */
    class Time : public DreamObject
    {

    private:
        /**
         * @brief Current time
         */
        long mCurrentFrameTime;

        /**
         * @brief Time of last frame
         */
        long mLastFrameTime;
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

        #ifdef DREAM_LOG
        /**
         * @brief Print Debug information about the Time object's state.
         */
        void show();
        #endif

        /**
         * @return Get the time at the point of function call. Different from
         * @see getCurrentFrameTime
         */
        long getAbsoluteTime();

        const static int DELTA_MAX;
    };

} // End of Dream
