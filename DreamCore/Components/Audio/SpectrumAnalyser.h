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
