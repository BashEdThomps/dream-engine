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
#include "fftw3.h"
#include <complex>
#include <vector>
#include "FrequencyRange.h"

using std::vector;

namespace Dream
{
    class AudioInstance;

    class SpectrumAnalyser : public DreamObject
    {
    public:
        explicit SpectrumAnalyser(AudioInstance* parent);
        ~SpectrumAnalyser() override;

        int getSampleRate() const;
        void setSampleRate(const int& audioFormat);

        void setParameters(int nfft, int overlap);

        const vector<float>& spectrum() const;

        float frequenceUnit() const;


        long onData(const char *data, long len);

        void runFFT();

        void debugSpectrum();

        bool getSourceStereo() const;
        void setSourceStereo(bool sourceStereo);

        void addFrequencyRange(const FrequencyRange&);
        void checkFrequencyRanges();

        void notifyFrequencyListeners(const FrequencyRange& range);

    private:
        AudioInstance* mParentHandle;
        vector<FrequencyRange> mFrequencyRanges;
        int mNumFFT;
        int mOverlap;
        int mSamplesNeeded;

        int mSampleRate;
        vector<float> mFFTIn;

        fftwf_plan mPlan;
        vector<std::complex<float>> mFFTOut;
        vector<float> mSpectrum;
        float mFrequencyUnit;
        bool mSourceStereo;
    };

}
