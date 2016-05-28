#ifndef MouseInputEvent_hpp
#define MouseInputEvent_hpp

#include "../InputEvent.h"

namespace Dream   {
namespace Plugins {
namespace Input   {
namespace GLFW    {
	
	enum MouseEventType {
		SCROLL,
		BUTTON,
		POSITION,
	};
	
	class MouseInputEvent : public Dream::Plugins::Input::InputEvent {
	private:
		MouseEventType mEventType;
		int mXScrollOffset;
		int mYScrollOffset;
		int mXPosition;
		int mYPosition;
		int mMods;
		int mButton;
		int mAction;
	public:
		MouseInputEvent(MouseEventType);
		~MouseInputEvent();
		
		int  getXScrollOffset();
		void setXScrollOffset(int);
		
		int  getYScrollOffset();
		void setYScrollOffset(int);
		
		void setXPosition(int);
		int  getXPosition();
		
		void setYPosition(int);
		int  getYPosition();
		
		int  getMods();
		void setMods(int);
		
		int getButton();
		void setButton(int);
		
		int getAction();
		void setAction(int);
		
		MouseEventType getMouseEventType();
	};
		
} // End of GLFWInput
} // End of Input
} // End of Plugins
} // End of Dream

#endif /* MouseInputEvent_hpp */
