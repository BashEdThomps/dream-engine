/*
* Dream::Asset::Instances::Scripting::Chai::ChaiScriptInstance
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ChaiScriptInstance.h"
#include "../../../../Plugins/Input/InputEvent.h"
#include "../../../../Plugins/Video/OpenGL/OGLVideo.h"
#include "../../../../Plugins/Video/OpenGL/Camera.h"
#include "../../../../Plugins/Video/OpenGL/GLFWTime.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Script    {
namespace Chai      {
    
	ChaiScriptInstance::ChaiScriptInstance(Dream::Asset::AssetDefinition* definition) :
		Dream::Asset::AssetInstance(definition) {
	}

	ChaiScriptInstance::~ChaiScriptInstance(void) {}
	
	bool ChaiScriptInstance::load(std::string projectPath) {
		mScript = new chaiscript::ChaiScript(chaiscript::Std_Lib::library());
		chaiscript::Boxed_Value result = mScript->eval_file(projectPath+mDefinition->getAssetPath());
		initInputEventInChaiScript();
		initGlfwTimeInChaiScript();
		initCameraInChaiScript();
		return !result.is_null();
	}
	
	void ChaiScriptInstance::initGlfwTimeInChaiScript() {
		mScript->add(chaiscript::user_type<Plugins::Video::OpenGL::GLFWTime>(),               "GLFWTime"      );
		mScript->add_global(chaiscript::var(Plugins::Video::OpenGL::GLFWTime::getInstance()), "gTime"         );
		mScript->add(chaiscript::fun(&Plugins::Video::OpenGL::GLFWTime::update),              "update"        );
		mScript->add(chaiscript::fun(&Plugins::Video::OpenGL::GLFWTime::getCurrentTime),      "getCurrentTime");
		mScript->add(chaiscript::fun(&Plugins::Video::OpenGL::GLFWTime::getLastTime),         "getLastTime"   );
		mScript->add(chaiscript::fun(&Plugins::Video::OpenGL::GLFWTime::getTimeDelta),        "getTimeDelta"  );
	}
	
	void ChaiScriptInstance::initCameraInChaiScript() {
		// Class Definition
		mScript->add(chaiscript::user_type<Plugins::Video::OpenGL::Camera>(),"Camera");
		// Methods
		mScript->add(chaiscript::fun(&Plugins::Video::OpenGL::Camera::processMouseScroll),   "processMouseScroll");
		mScript->add(chaiscript::fun(&Plugins::Video::OpenGL::Camera::processKeyboard),      "processKeyboard");
		mScript->add(chaiscript::fun(&Plugins::Video::OpenGL::Camera::processMouseMovement), "processMouseMovement");
		// Global Variables
		mScript->add_global(chaiscript::const_var(CAMERA_MOVEMENT_FORWARD),  "CAMERA_MOVEMENT_FORWARD");
		mScript->add_global(chaiscript::const_var(CAMERA_MOVEMENT_BACKWARD), "CAMERA_MOVEMENT_BACKWARD");
		mScript->add_global(chaiscript::const_var(CAMERA_MOVEMENT_LEFT),     "CAMERA_MOVEMENT_LEFT");
		mScript->add_global(chaiscript::const_var(CAMERA_MOVEMENT_RIGHT),    "CAMERA_MOVEMENT_RIGHT");
		// Camera Instance
		mScript->add_global(chaiscript::var(&Plugins::Video::OpenGL::OGLVideo::sCamera),"GLCamera");
		
	}
	
	void ChaiScriptInstance::initInputEventInChaiScript() {
		// Class Definition
		mScript->add(chaiscript::user_type<Dream::Plugins::Input::InputEvent>(),     "InputEvent");
		// Methods
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getSource),        "getSource");
		
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getXPosition),     "getXPosition");
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getYPosition),     "getYPosition");
		
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getXPositionOffset), "getXPositionOffset");
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getYPositionOffset), "getYPositionOffset");
		
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getXScrollOffset), "getXScrollOffset");
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getYScrollOffset), "getYScrollOffset");
		
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getKey),           "getKey");
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getMods),          "getMods");
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getAction),        "getAction");
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getButton),        "getButton");
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getScancode),      "getScancode");
		// Constants
		mScript->add_global(chaiscript::const_var(INPUT_TYPE_JOYSTICK),       "INPUT_TYPE_JOYSTICK");
		mScript->add_global(chaiscript::const_var(INPUT_TYPE_KEYBOARD),       "INPUT_TYPE_KEYBOARD");
		mScript->add_global(chaiscript::const_var(INPUT_TYPE_MOUSE_BUTTON),   "INPUT_TYPE_MOUSE_BUTTON");
		mScript->add_global(chaiscript::const_var(INPUT_TYPE_MOUSE_POSITION), "INPUT_TYPE_MOUSE_POSITION");
		mScript->add_global(chaiscript::const_var(INPUT_TYPE_MOUSE_SCROLL),   "INPUT_TYPE_MOUSE_SCROLL");
		// Input Actions
		mScript->add_global(chaiscript::const_var(GLFW_PRESS),   "INPUT_PRESS");
		mScript->add_global(chaiscript::const_var(GLFW_RELEASE), "INPUT_RELEASE");
		mScript->add_global(chaiscript::const_var(GLFW_REPEAT),  "INPUT_REPEAT");
		// Cursor Keys
		mScript->add_global(chaiscript::const_var(GLFW_KEY_UP),   "KEY_UP");
		mScript->add_global(chaiscript::const_var(GLFW_KEY_DOWN), "KEY_DOWN");
		mScript->add_global(chaiscript::const_var(GLFW_KEY_LEFT), "KEY_LEFT");
		mScript->add_global(chaiscript::const_var(GLFW_KEY_RIGHT),"KEY_RIGHT");
		// WASD
		mScript->add_global(chaiscript::const_var(GLFW_KEY_W),"KEY_W");
		mScript->add_global(chaiscript::const_var(GLFW_KEY_A),"KEY_A");
		mScript->add_global(chaiscript::const_var(GLFW_KEY_S),"KEY_S");
		mScript->add_global(chaiscript::const_var(GLFW_KEY_D),"KEY_D");
	}
	
	void ChaiScriptInstance::initSceneObjectInChaiScript() {
		mScript->add(chaiscript::user_type<Dream::Scene::SceneObject>(),"SceneObject");
	}
	
	void ChaiScriptInstance::update() {
		auto updateFunction = mScript->eval<std::function<void(void)>>("update");
		updateFunction();
	}
	
	void ChaiScriptInstance::processInputs(std::vector<Plugins::Input::InputEvent> inputEventVector) {
		for(std::vector<Plugins::Input::InputEvent>::iterator it = inputEventVector.begin(); it != inputEventVector.end(); it++) {
			auto processFunction = mScript->eval<std::function<void(Plugins::Input::InputEvent)>>("onInputEvent");
			processFunction(*it);
			Plugins::Input::InputEvent::LastEvent = (*it);
		}
	}
	
} // End of ChaiScript
} // End of Script
} // End of Instance
} // End of Asset
} // End of Dream
