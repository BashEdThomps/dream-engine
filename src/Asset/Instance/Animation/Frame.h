#ifndef DA_FRAME_H
#define DA_FRAME_H

#include <vector>
#include "FrameDelta.h"

namespace Dream {
	namespace Asset {
		namespace Instance {
			namespace Animation {
				class Frame {
				private:
				  int mIndex;
				  std::vector<FrameDelta*> mFrameDeltas;
				public:
					Frame(int);
					~Frame();
					void addFrameDelta(FrameDelta*);
					int compareIndecies(Frame*);
					int getNumFrameDeltas();
					int getIndex();
					std::vector<FrameDelta*> getFrameDeltas();
				};
			}
		}
	}
}

#endif // DA_FRAME
