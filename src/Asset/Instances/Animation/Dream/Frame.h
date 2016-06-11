#ifndef DA_FRAME_H
#define DA_FRAME_H

#include <vector>
#include "../../../../Scene/SceneObject.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Animation {
namespace Dream     {
	
	class Frame {
	private:
		std::vector<float> mTranslation;
		std::vector<float> mRotation;
		std::vector<float> mScale;
	public:
		Frame();
		~Frame();
		void applyToSceneObject(Scene::SceneObject*);
	}; // End Frame
	
} // End of Dream
} // End of Animation
} // End of Instances
} // End of Asset
} // End of Dream

#endif // DA_FRAME
