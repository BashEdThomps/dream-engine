//
//  GLFWInput.hpp
//  Dream
//
//  Created by Ashley Thompson on 16/05/2016.
//  Copyright © 2016 Octronic. All rights reserved.
//

#ifndef GLFWInput_hpp
#define GLFWInput_hpp

#include "../Interfaces/InputInterface.h"
#include <GLFW/glfw3.h>

namespace Dream   {
namespace Plugins {
namespace Input   {
	
	class GLFWInput : public InputInterface {
	public:
		GLFWInput();
		~GLFWInput();
		bool init();
		void update(Dream::Scene::Scene*);
	};
	
} // End of Input
} // End of Plugins
} // End of Dream

#endif /* GLFWInput_hpp */
