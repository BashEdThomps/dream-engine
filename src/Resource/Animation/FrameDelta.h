#ifndef DA_FRAMEDELTA_H
#define DA_FRAMEDELTA_H

#include "Constants.h"

namespace Dream {
	namespace Resource {
		namespace Animation {
			//! TODO - Document
			class FrameDelta {
			private:
				int   mDrawableID;
				float mPositionDelta [3];
				float mRotationDelta [3];
				float mScaleDelta    [3];
				float mOrbitRadius;
				int   mOperation;
			public:
				//! Create frame delta with drawableID and Operation
				FrameDelta(int, int);
				~FrameDelta();
				//! TODO - Document
				FrameDelta* computeFrameDelta(FrameDelta*, FrameDelta*, int, int);
				//! TODO - Document
				void computeDeltaVector(float*, float*, float*);
				//! TODO - Document
				void printDebug();
				//! TODO - Document
				int getDrawableID();
				//! TODO - Document
				float* getPositionDelta();
				//! TODO - Document
				float* getRotationDelta();
				//! TODO - Document
				int getOperation();
				//! TODO - Document
				void setPositionDelta(float*);
				//! TODO - Document
				void setRotationDelta(float*);
			};
		}
	}
}
#endif // DA_FRAMEDELTA_H
