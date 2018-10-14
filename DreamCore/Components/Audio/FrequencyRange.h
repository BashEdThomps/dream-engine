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

#include "../../Common/DreamObject.h"
#include <vector>

using std::vector;

namespace Dream
{
    class FrequencyRange : public DreamObject
    {
    public:
        FrequencyRange(float min, float max, float threshold);
        ~FrequencyRange();

        bool test(float freqUnit, const vector<float>& data);
        bool isFrequencyInRange(float freq) const;

        float getMin() const;
        void setMin(float min);

        float getMax() const;
        void setMax(float max);

        float getAmplitude() const;

        float getThreshold() const;
        void setThreshold(float threshold);

    private:
        float mMin;
        float mMax;
        float mThreshold;
        float mAmplitude;
    };
}
