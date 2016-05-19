#include <iostream>
#include "Frame.h"

namespace Dream          {
namespace Asset          {
namespace Instances      {
namespace Animation      {
namespace DreamAnimation {
	
	Frame::Frame(int index) {
		mIndex = index;
	}

    void  Frame::addFrameDelta(FrameDelta* frameDelta) {
      mFrameDeltas.push_back(frameDelta);
      return;
    }

    int Frame::compareIndecies(Frame *frame2) {
      return frame2->getIndex() - getIndex();
    }

    int Frame::getNumFrameDeltas() {
      return mFrameDeltas.size();
    }

    int Frame::getIndex() {
      return mIndex;
    }

    std::vector<FrameDelta*> Frame::getFrameDeltas() {
      return mFrameDeltas;
    }

} // End of Dream
} // End of Animation
} // End of Instance
} // End of Asset
} // End of Dream
