#include "LightInstance.h"

namespace Dream {
  namespace Components {
    namespace Video {
      LightInstance::LightInstance(AssetDefinition* definition) : AssetInstance(definition) {}

      LightInstance::~LightInstance() {}

      bool LightInstance::load(std::string projectDir) {
        return false;
      }
    } // End of Video
  } // End of Components
} // End of Dream
