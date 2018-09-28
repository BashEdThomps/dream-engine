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
