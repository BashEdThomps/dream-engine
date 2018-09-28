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
