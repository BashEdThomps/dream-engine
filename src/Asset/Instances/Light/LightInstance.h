#ifndef LightInstance_h
#define LightInstance_h

#include "../../AssetDefinition.h"
#include "../../AssetInstance.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Light     {
	
	class LightInstance : public AssetInstance {
	public:
		LightInstance(AssetDefinition*);
		~LightInstance();
		bool load(std::string);
	}; // End of LightInstance
	
} // End of Light
} // End of Instances
} // End of Asset
} // End of Dream

#endif /* LightInstance_h */
