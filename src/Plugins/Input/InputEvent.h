#ifndef InputEvent_hpp
#define InputEvent_hpp

namespace Dream   {
namespace Plugins {
namespace Input   {
	
	enum InputEventSource {
		MOUSE_POSITION,
		MOUSE_SCROLL,
		MOUSE_BUTTON,
		KEYBOARD,
		JOYSTICK
	};
	
	class InputEvent {
	private:
		int mKey;
		int mScancode;
		int mAction;
		int mMods;
		int mXScrollOffset;
		int mYScrollOffset;
		int mXPosition;
		int mYPosition;
		int mButton;
		InputEventSource mSource;
	public:
		InputEvent(InputEventSource source);
		~InputEvent();
		
		int  getMods();
		void setMods(int);
		
		int getKey();
		void setKey(int);
		
		int getAction();
		void setAction(int);
		
		int getScancode();
		void setScancode(int);
		
		int  getXScrollOffset();
		void setXScrollOffset(int);
		
		int  getYScrollOffset();
		void setYScrollOffset(int);
		
		void setXPosition(int);
		int  getXPosition();
		
		void setYPosition(int);
		int  getYPosition();
		
		int getButton();
		void setButton(int);
		
		InputEventSource getSource();
	};
			
}
}
}

#endif /* InputEvent_hpp */
