#ifndef DA_KEYFRAME_H
#define DA_KEYFRAME_H

#include "Constants.h"
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
				FrameDelta* mDeltas[DA_DELTA_SZ];
				//! TODO - Document
				long mDuration;
				//! TODO - Document
				int mWrap;
			public:
				//! TODO - Document
				KeyFrame(int, long);
				//! TODO - Document
				~KeyFrame();
				//! TODO - Document
				void addDelta(FrameDelta*);
				//! TODO - Document
				int getNextAvailableIndex();
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
