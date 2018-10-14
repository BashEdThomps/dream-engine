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
    class Time : public DreamObject
    {
    private:
        time_point<high_resolution_clock> mCurrentTime;
        time_point<high_resolution_clock> mLastTime;
        duration<double> mTimeDelta;
    public:
        Time();
        ~Time();
        double getCurrentFrameTime();
        double getLastFrameTime();
        double getFrameTimeDelta();
        double scaleValueByFrameTime(double value);
        void updateFrameTime();
        void show();
        double now();
        long long nowLL();
    };

} // End of Dream
