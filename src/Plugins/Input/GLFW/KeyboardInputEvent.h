#ifndef KeyboardInputEvent_hpp
#define KeyboardInputEvent_hpp

#include "../InputEvent.h"

namespace Dream   {
namespace Plugins {
namespace Input   {
namespace GLFW    {
	
	class KeyboardInputEvent : public Dream::Plugins::Input::InputEvent{
	private:
		int mKey;
		int mScancode;
		int mAction;
		int mMods;
	public:
		KeyboardInputEvent();
		~KeyboardInputEvent();
		
		int  getMods();
		void setMods(int);
		
		int getKey();
		void setKey(int);
		
		int getAction();
		void setAction(int);
		
		int getScancode();
		void setScancode(int);
	};
	
} // End of GLFWInput
} // End of Input
} // End of Plugins
} // End of Dream

#endif /* InputEvent_hpp */
