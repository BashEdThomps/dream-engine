#ifndef DA_FRAMEDELTA_H
#define DA_FRAMEDELTA_H

//! Delta Operations
#define DA_OP_LINEAR 0
#define DA_OP_BEZIER 1
#define DA_OP_ORBIT  2

namespace Dream {
	namespace Resource {
		namespace Instance {
			namespace Animation {
				class FrameDelta {
				private:
					int   mDrawableID;
					float mPositionDelta [3];
					float mRotationDelta [3];
					float mScaleDelta    [3];
					float mOrbitRadius;
					int   mOperation;
				public:
					FrameDelta(int, int);
					~FrameDelta();
					FrameDelta* computeFrameDelta(FrameDelta*, FrameDelta*, int, int);
					void computeDeltaVector(float*, float*, float*);
					void printDebug();
					int getDrawableID();
					float* getPositionDelta();
					float* getRotationDelta();
					int getOperation();
					void setPositionDelta(float*);
					void setRotationDelta(float*);
				};
			}
		}
	}
}
#endif // DA_FRAMEDELTA_H
