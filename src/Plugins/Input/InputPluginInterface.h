
#ifndef InputInterface_hpp
#define InputInterface_hpp

#include "../PluginInterface.h"
#include "InputEvent.h"
#include <vector>

namespace Dream   {
namespace Plugins {
namespace Input   {
	
	class InputPluginInterface : public Dream::Plugins::PluginInterface {
	public:
		InputPluginInterface();
		virtual ~InputPluginInterface() {}
		
		static void pushEvent(InputEvent);
		static std::vector<InputEvent> getInputEventVector();
		static void clearInputEvents();
		
	}; // End of InputInterface
	
} // End of Input
} // End of Plugins
} // End of Dream

#endif /* InputInterface_hpp */
