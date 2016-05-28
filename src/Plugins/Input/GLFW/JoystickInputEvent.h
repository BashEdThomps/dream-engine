#ifndef JoystickInputEvent_hpp
#define JoystickInputEvent_hpp

#include "../InputEvent.h"

namespace Dream   {
namespace Plugins {
namespace Input   {
namespace GLFW    {
	
	class JoystickInputEvent : public InputEvent {
	public:
		JoystickInputEvent();
		~JoystickInputEvent();
	};
	
} // End of GLFWInput
} // End of Input
} // End of Plugins
} // End of Dream

#endif /* JoystickInputEvent_hpp */
