#ifndef DA_FRAME_H
#define DA_FRAME_H

#include "FrameDelta.h"

#define DA_FRAME_DELTA_SZ 1024

namespace Dream {
	namespace Animation {
		//! TODO - Document
		class Frame {
		private:
		    int         mIndex;
		    FrameDelta* mFrameDeltas[DA_FRAME_DELTA_SZ];
		public:
			//! TODO - Document
			Frame(int);
			//! TODO - Document
			~Frame();
			//! TODO - Document
			void addFrameDelta(FrameDelta*);
			//! TODO - Document
			int getNextAvailableFrameDeltaIndex();
			//! TODO - Document
			int compareIndecies(Frame*);
			//! TODO - Document
			int getNumFrameDeltas();
			//! TODO - Document
			int getIndex();
			//! TODO - Document
			FrameDelta** getFrameDeltas();

		};
	}
}

#endif // DA_FRAME
