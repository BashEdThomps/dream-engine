#include "KeyboardInputEvent.h"

namespace Dream   {
namespace Plugins {
namespace Input   {
namespace GLFW    {
		
	KeyboardInputEvent::KeyboardInputEvent() :
		Dream::Plugins::Input::InputEvent() {
		
	}
	
	KeyboardInputEvent::~KeyboardInputEvent() {}
	
	int  KeyboardInputEvent::getMods() {
		return mMods;
	}
	
	void KeyboardInputEvent::setMods(int mods) {
		mMods = mods;
	}
	
	int KeyboardInputEvent::getKey() {
		return mKey;
	}
	
	void KeyboardInputEvent::setKey(int key) {
		mKey = key;
	}
	
	int KeyboardInputEvent::getAction() {
		return mAction;
	}
	
	void KeyboardInputEvent::setAction(int action) {
		mAction = action;
	}
	
	int KeyboardInputEvent::getScancode() {
		return mScancode;
	}
	
	void KeyboardInputEvent::setScancode(int scancode) {
		mScancode = scancode;
	}
	
} // End of GLFWInput
} // End of Input
} // End of Plugins
} // End of Dream
