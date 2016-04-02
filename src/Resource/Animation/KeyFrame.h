#ifndef DA_KEYFRAME_H
#define DA_KEYFRAME_H

#include <vector>
#include "FrameDelta.h"

namespace Dream {
	namespace Resource {
		namespace Animation {
			//! TODO - Document
			class KeyFrame {
			private:
				//! TODO - Document
				int mIndex;
				//! TODO - Document
				std::vector<FrameDelta*> mDeltas;
				//! TODO - Document
				long mDuration;
				//! TODO - Document
				int mWrap;
				//! TODO - Document
				int mFramesPerSecond;
			public:
				//! TODO - Document
				KeyFrame(int, int, long);
				//! TODO - Document
				~KeyFrame();
				//! TODO - Document
				void addDelta(FrameDelta*);
				//! TODO - Document
				int getNumberOfFrameDeltas();
				//! TODO - Document
				int getIntermediateFrameCount();
				//! TODO - Document
				int compareIndicies(KeyFrame*);
				//! TODO - Document
				FrameDelta* getDeltaByDrawableID(int);
				//! TODO - Document
				int getWrap();
				//! TODO - Document
				int getIndex();
			};
		}
	}
}
#endif // DA_KEYFRAME_H
