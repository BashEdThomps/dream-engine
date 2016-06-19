#ifndef FRAME_H
#define FRAME_H

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
		
		std::vector<float> getTranslation();
		void               setTranslation(std::vector<float>);
		
		std::vector<float> getRotation();
		void               setRotation(std::vector<float>);
		
		std::vector<float> getScale();
		void               setScale(std::vector<float>);
		
		void showStatus();
		
	}; // End Frame
	
} // End of Dream
} // End of Animation
} // End of Instances
} // End of Asset
} // End of Dream

#endif // FRAME_H
