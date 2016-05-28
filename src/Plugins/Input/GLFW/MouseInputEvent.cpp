#include "MouseInputEvent.h"

namespace Dream   {
namespace Plugins {
namespace Input   {
namespace GLFW    {
	
	MouseInputEvent::MouseInputEvent(MouseEventType type) : Dream::Plugins::Input::InputEvent::InputEvent() {
		mEventType = type;
		mMods = 0;
		
		mXPosition = 0;
		mYPosition = 0;
		
		mXScrollOffset = 0;
		mYScrollOffset = 0;
	}
	
	MouseInputEvent::~MouseInputEvent() {}
	
	
	int  MouseInputEvent::getXScrollOffset() {
		return mXScrollOffset;
	}
	
	void MouseInputEvent::setXScrollOffset(int xOffset) {
		mXScrollOffset = xOffset;
	}
		
	int  MouseInputEvent::getYScrollOffset() {
		return mYScrollOffset;
	}
	
	void MouseInputEvent::setYScrollOffset(int yOffset) {
		mYScrollOffset = yOffset;
	}
	
	void MouseInputEvent::setXPosition(int xPosition) {
		mXPosition = xPosition;
	}
	
	int MouseInputEvent::getXPosition() {
		return mXPosition;
	}
	
	void MouseInputEvent::setYPosition(int yPosition) {
		mYPosition = yPosition;
	}
	
	int MouseInputEvent::getYPosition() {
		return mYPosition;
	}
		
	int  MouseInputEvent::getMods() {
		return mMods;
	}
	
	void MouseInputEvent::setMods(int mods) {
		mMods = mods;
	}
	
	int MouseInputEvent::getButton() {
		return mButton;
	}
	
	void MouseInputEvent::setButton(int button) {
		mButton = button;
	}
		
	int MouseInputEvent::getAction() {
		return mAction;
	}
	
	void MouseInputEvent::setAction(int action) {
		mAction = action;
	}
		
} // End of GLFWInput
} // End of Input
} // End of Plugins
} // End of Dream
