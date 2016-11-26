#include "LightInstance.h"

namespace Dream {
      LightInstance::LightInstance(
          AssetDefinition* definition,
          Transform3D* transform
      ) : AssetInstance(definition,transform) {
        return;
      }

      LightInstance::~LightInstance() {
        return;
      }

      bool LightInstance::load(string projectDir) {
        return false;
      }
} // End of Dream
