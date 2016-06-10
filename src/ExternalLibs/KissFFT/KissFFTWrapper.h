#ifndef KissFFTWrapper_h
#define KissFFTWrapper_h

#include "kissfft.hh"


typedef kissfft<double> FFT;
typedef std::complex<double> cpx_type;

#define NUM_FFT 1024


namespace Dream   {
namespace Extra   {
namespace KissFFT {
	
	class KissFFTWrapper {
	private:
		FFT                   *mFFT;
		std::vector<cpx_type> mInputBuffer;
		std::vector<cpx_type> mOutputBuffer;
		static KissFFTWrapper *mInstance;
		KissFFTWrapper();
		~KissFFTWrapper();
	public:
		static KissFFTWrapper *getInstance();
		void                   transformFFT();
		void                   insertIntoInputBuffer(cpx_type*,int);
		std::vector<cpx_type>  getOutputBuffer();
		void                   clearBuffers();
		
	}; // End of KissFFTWarapper
			
} // End of KissFFT
} // End of Extra
} // End of Dream

#endif /* KissFFTWrapper_h */
