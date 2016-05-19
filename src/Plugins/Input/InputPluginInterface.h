
#ifndef InputInterface_hpp
#define InputInterface_hpp

#include "../PluginInterface.h"

namespace Dream   {
namespace Plugins {
namespace Input   {
	
	class InputPluginInterface : public Dream::Plugins::PluginInterface {
	public:
		InputPluginInterface();
		~InputPluginInterface();
	}; // End of InputInterface
	
} // End of Input
} // End of Plugins
} // End of Dream

#endif /* InputInterface_hpp */
