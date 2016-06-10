#include "KissFFTWrapper.h"

namespace Dream   {
namespace Extra   {
namespace KissFFT {
	
	KissFFTWrapper *KissFFTWrapper::mInstance = NULL;
	
	KissFFTWrapper* KissFFTWrapper::getInstance() {
		if (mInstance == NULL) {
			mInstance = new KissFFTWrapper();
		}
		return mInstance;
	}
	
	KissFFTWrapper::KissFFTWrapper() {
		mInputBuffer = std::vector<cpx_type>(NUM_FFT);
		mOutputBuffer = std::vector<cpx_type>(NUM_FFT);
		mFFT = new FFT(NUM_FFT,false);
	}
	
	KissFFTWrapper::~KissFFTWrapper() {
		delete mFFT;
	}
	
	void KissFFTWrapper::insertIntoInputBuffer(cpx_type* data, int nSamples) {
		mInputBuffer.insert(mInputBuffer.end(), data, data+nSamples);
	}
	
	std::vector<cpx_type> KissFFTWrapper::getOutputBuffer() {
		return mOutputBuffer;
	}
	
	void KissFFTWrapper::clearBuffers() {
		mInputBuffer.clear();
		mOutputBuffer.clear();
	}
	
	void KissFFTWrapper::transformFFT() {
		mFFT->transform(&mInputBuffer[0], &mOutputBuffer[0]);
	}
	
	
} // End of KissFFT
} // End of Extra
} // End of Dream