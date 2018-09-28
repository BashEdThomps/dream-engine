#include "SpectrumAnalyser.h"
#include "AudioInstance.h"

namespace Dream
{
    SpectrumAnalyser::SpectrumAnalyser
    (AudioInstance* parent) :
        DreamObject("SpectrumAnalyser"),
        mParentHandle(parent),
        mFrequencyUnit(0),
        mSourceStereo(false),
        mPlan(nullptr)
    {
        addFrequencyRange(FrequencyRange(40.0f,60.0f,10.0f));
    }

    SpectrumAnalyser::~SpectrumAnalyser
    ()
    {
        if (mPlan != nullptr)
        {
            fftwf_destroy_plan(mPlan);
            mPlan = nullptr;
        }
    }

    long
    SpectrumAnalyser::onData
    (const char *data, long len)
    {
        // 16bit = 2bytes (len / 2) or double for Stereo
        int samplesReady = len >> (mSourceStereo ? 2 : 1);
        const short* ptr = reinterpret_cast<const short*>(data);
        while (samplesReady > 0)
        {
            mFFTIn[mNumFFT - mSamplesNeeded] = (*ptr) * (1.0 / 65536); // normalized to -1.0 ~ +1.0
            --mSamplesNeeded;
            --samplesReady;
            ++ptr;
            if (mSourceStereo)
            {
                ++ptr;
            }
            if (mSamplesNeeded == 0)
            {
                runFFT();
                mSamplesNeeded = mOverlap;
                std::copy(mFFTIn.begin() + mOverlap, mFFTIn.end(), mFFTIn.begin());
            }
        }

        return len - samplesReady;
    }
    void
    SpectrumAnalyser::runFFT
    ()
    {
        fftwf_execute(mPlan);
        for (size_t i = 0; i < mFFTOut.size(); ++i)
        {
            // The norm value of a complex number is the squared magnitude
            //mSpectrum[i] = std::norm(mFFTOut[i] / float(mNumFFT));
            auto real = mFFTOut[i].real();
            auto imag = mFFTOut[i].imag();

            mSpectrum[i] = sqrt(real*real + imag+imag);
        }
        checkFrequencyRanges();
        //debugSpectrum();
    }

    void
    SpectrumAnalyser::debugSpectrum
    ()
    {
        auto log=getLog();
        cout << "--------------------" << endl;
        for (size_t i=0; i<mSpectrum.size(); i++)
        {
            auto val =  mSpectrum[i];
            if (val > 0.001)
            {
                cout << i*mFrequencyUnit  << "Hz\t" << val << endl;
            }
        }
    }

    bool
    SpectrumAnalyser::getSourceStereo
    ()
    const
    {
        return mSourceStereo;
    }

    void SpectrumAnalyser::setSourceStereo(bool sourceStereo)
    {
        mSourceStereo = sourceStereo;
    }

    void
    SpectrumAnalyser::addFrequencyRange
    (const FrequencyRange& range)
    {
        mFrequencyRanges.push_back(range);
    }

    void
    SpectrumAnalyser::checkFrequencyRanges
    ()
    {
        for (FrequencyRange& range : mFrequencyRanges)
        {
           if (range.test(mFrequencyUnit, mSpectrum))
           {
               notifyFrequencyListeners(range);
           }
        }
    }

    void
    SpectrumAnalyser::notifyFrequencyListeners
    (const FrequencyRange& range)
    {
        cout << "Notification of frequency spike for "
             << range.getMin() << "Hz-" << range.getMax() << "Hz"
             << " with amplitude " << range.getAmplitude()
             << endl;
    }

    int
    SpectrumAnalyser::getSampleRate
    ()
    const
    {
        return mSampleRate;
    }

    void
    SpectrumAnalyser::setSampleRate
    (const int &audioFormat)
    {
        mSampleRate = audioFormat;
    }

    void
    SpectrumAnalyser::setParameters
    (int nfft, int overlap)
    {
        mNumFFT = nfft;
        mOverlap = overlap;
        mFFTIn.resize(mNumFFT);
        fill(mFFTIn.begin(), mFFTIn.begin()+mNumFFT,0.0);
        mSamplesNeeded = overlap;
        mFFTOut.resize(static_cast<size_t>(mNumFFT) / 2 + 1);
        mSpectrum.resize(mFFTOut.size());
        mPlan = fftwf_plan_dft_r2c_1d(mNumFFT, mFFTIn.data(), reinterpret_cast<fftwf_complex*>(mFFTOut.data()), FFTW_PATIENT);
        mFrequencyUnit = static_cast<float>(mSampleRate) / static_cast<float>(mNumFFT);
        cout << "\t FFT Params:\n"
             << "\t\tSampleRate: " << mSampleRate << endl
             << "\t\tmNumFFT: " << mNumFFT << endl
             << "\t\tFrequencyUnit:" << mFrequencyUnit
             << endl;
    }

    const vector<float>
    &SpectrumAnalyser::spectrum
    ()
    const
    {
        return mSpectrum;
    }

    float
    SpectrumAnalyser::frequenceUnit
    () const
    {
        return mFrequencyUnit;
    }
}
