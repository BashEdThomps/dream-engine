#ifndef FRAMEDELTA_H
#define FRAMEDELTA_H

//! Delta Operations
#define FRAME_DELTA_OP_LINEAR 0
#define FRAME_DELTA_OP_BEZIER 1
#define FRAME_DELTA_OP_ORBIT  2

namespace Dream          {
namespace Asset          {
namespace Instances      {
namespace Animation      {
namespace DreamAnimation {
	
	class FrameDelta {
	private:
		int   mDrawableID;
		float mPositionDelta [3];
		float mRotationDelta [3];
		float mScaleDelta    [3];
		float mOrbitRadius;
		int   mOperation;
	public:
		FrameDelta  (int, int);
		~FrameDelta ();
		FrameDelta* computeFrameDelta(FrameDelta*, FrameDelta*, int, int);
		void        computeDeltaVector(float*, float*, float*);
		void        showStatus();
		int         getDrawableID();
		float*      getPositionDelta();
		float*      getRotationDelta();
		int         getOperation();
		void        setPositionDelta(float*);
		void        setRotationDelta(float*);
	};
	
} // End of Dream
} // End of Animation
} // End of Instance
} // End of Asset
} // End of Dream

#endif // FRAMEDELTA_H
