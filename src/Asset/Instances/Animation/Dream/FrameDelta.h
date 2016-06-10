#ifndef FRAMEDELTA_H
#define FRAMEDELTA_H

#include <vector>

//! Delta Operations
#define FRAME_DELTA_OP_LINEAR 0
#define FRAME_DELTA_OP_BEZIER 1
#define FRAME_DELTA_OP_ORBIT  2

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Animation {
namespace Dream     {
	
	class FrameDelta {
	private:
		int   mDrawableID;
		std::vector<float> mPositionDelta;
		std::vector<float> mRotationDelta;
		std::vector<float> mScaleDelta;
		float mOrbitRadius;
		int   mOperation;
	public:
		FrameDelta  (int, int);
		~FrameDelta ();
		FrameDelta* computeFrameDelta(FrameDelta*, FrameDelta*, int, int);
		void        computeDeltaVector(std::vector<float>, std::vector<float>, std::vector<float>);
		void        showStatus();
		int         getDrawableID();
		std::vector<float> getPositionDelta();
		std::vector<float> getRotationDelta();
		std::vector<float> getScaleDelta();
		int         getOperation();
		void        setPositionDelta(std::vector<float>);
		void        setRotationDelta(std::vector<float>);
		void        setScaleDelta(std::vector<float>);
	};
	
} // End of Dream
} // End of Animation
} // End of Instances
} // End of Asset
} // End of Dream

#endif // FRAMEDELTA_H
