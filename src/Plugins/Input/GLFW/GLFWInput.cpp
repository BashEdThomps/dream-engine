//
//  GLFWInput.cpp
//  Dream
//
//  Created by Ashley Thompson on 16/05/2016.
//  Copyright © 2016 Octronic. All rights reserved.
//

#include "GLFWInput.h"

namespace Dream   {
namespace Plugins {
namespace Input   {
namespace GLFW    {
	
	GLFWInput::GLFWInput() : Dream::Plugins::Input::InputInterface() {
		
	}
	
	GLFWInput::~GLFWInput() {
		
	}
	
	bool GLFWInput::init() {
		std::cout << "GLFWInput: No Initialisation Required." << std::endl;
		return true;
	}
	
	void GLFWInput::update(Dream::Scene::Scene *scene) {
		glfwPollEvents();
	}
	
} // End of GLFW
} // End of Input
} // End of Plugins
} // End of Dream