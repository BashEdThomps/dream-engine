#ifndef DA_FRAME_H
#define DA_FRAME_H

#include <vector>
#include "FrameDelta.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Animation {
namespace Dream     {
	
	class Frame {
	private:
		int                      mIndex;
		std::vector<FrameDelta*> mFrameDeltas;
	public:
		Frame(int);
		~Frame();
		void addFrameDelta(FrameDelta*);
		int  compareIndecies(Frame*);
		int  getNumFrameDeltas();
		int  getIndex();
		std::vector<FrameDelta*> getFrameDeltas();
	}; // End Frame
	
} // End of Dream
} // End of Animation
} // End of Instances
} // End of Asset
} // End of Dream

#endif // DA_FRAME
