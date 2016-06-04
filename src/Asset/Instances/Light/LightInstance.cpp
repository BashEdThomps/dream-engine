#include "LightInstance.h"

namespace Dream {
namespace Asset {
namespace Instances {
namespace Light {
	
	LightInstance::LightInstance(AssetDefinition* definition) : AssetInstance(definition) {
		
	}
	
	LightInstance::~LightInstance() {
		
	}
	
	bool LightInstance::load(std::string projectDir) {
		return false;
	}

} // End of Light
} // End of Instances
} // End of Asset
} // End of Dream