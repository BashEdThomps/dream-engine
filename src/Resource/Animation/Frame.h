#ifndef DA_FRAME_H
#define DA_FRAME_H

#include <vector>
#include "FrameDelta.h"

namespace Dream {
	namespace Resource {
		namespace Animation {
			//! TODO - Document
			class Frame {
			private:
			    int         mIndex;
			    std::vector<FrameDelta*> mFrameDeltas;
			public:
				//! TODO - Document
				Frame(int);
				//! TODO - Document
				~Frame();
				//! TODO - Document
				void addFrameDelta(FrameDelta*);
				//! TODO - Document
				int compareIndecies(Frame*);
				//! TODO - Document
				int getNumFrameDeltas();
				//! TODO - Document
				int getIndex();
				//! TODO - Document
				std::vector<FrameDelta*> getFrameDeltas();

			};
		}
	}
}

#endif // DA_FRAME
