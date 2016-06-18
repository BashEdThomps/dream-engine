//
//  GLFWInput.hpp
//  Dream
//
//  Created by Ashley Thompson on 16/05/2016.
//  Copyright © 2016 Octronic. All rights reserved.
//

#ifndef GLFWInput_hpp
#define GLFWInput_hpp

#include <vector>
#include <GLFW/glfw3.h>

#include "../InputComponentInterface.h"
#include "../InputEvent.h"

namespace Dream   {
namespace Components {
namespace Input   {
namespace GLFW {
	
	class GLFWInput : public InputComponentInterface {
	private:
		GLFWwindow *mWindow;
	public:
		GLFWInput();
		~GLFWInput();
		void setWindow(GLFWwindow*);
		bool init();
		void update(Dream::Scene::Scene*);
	};
	
} // End of GLFW
} // End of Input
} // End of Components
} // End of Dream

#endif /* GLFWInput_hpp */
