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


#include "FrequencyRange.h"

#include <iostream>
using std::cout;
using std::endl;

namespace Dream
{
    FrequencyRange::FrequencyRange
    (float min, float max, float threshold)
        : DreamObject ("FrequencyRange"),
          mMin(min),
          mMax(max),
          mThreshold(threshold),
          mAmplitude(0.0f)
    {

    }

    FrequencyRange::~FrequencyRange
    ()
    {

    }

    bool
    FrequencyRange::test
    (float freqUnit, const vector<float>& data)
    {
        size_t indexMin = static_cast<size_t>(mMin/freqUnit);
        size_t indexMax = static_cast<size_t>(mMax/freqUnit);

        mAmplitude = 0.0f;
        for (size_t i=indexMin; i < indexMax; i++)
        {
            mAmplitude += data[i];
        }

        /*
        cout << "Testing from " << indexMin
             << " to " << indexMax
             << " got " << mAmplitude
             << endl;
             */

        return mAmplitude >= mThreshold;
    }

    bool
    FrequencyRange::isFrequencyInRange
    (float freq)
    const
    {
        return freq >= mMin && freq <= mMax;
    }

    float FrequencyRange::getMin() const
    {
        return mMin;
    }

    float FrequencyRange::getMax() const
    {
        return mMax;
    }

    float FrequencyRange::getAmplitude() const
    {
        return mAmplitude;
    }

    float FrequencyRange::getThreshold() const
    {
        return mThreshold;
    }

    void FrequencyRange::setThreshold(float threshold)
    {
        mThreshold = threshold;
    }

    void FrequencyRange::setMin(float min)
    {
        mMin = min;
    }

    void FrequencyRange::setMax(float max)
    {
        mMax = max;
    }
}
