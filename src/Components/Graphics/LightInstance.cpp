#include "LightInstance.h"

namespace Dream {
  namespace Components {
    namespace Graphics {
      LightInstance::LightInstance(
          AssetDefinition* definition,
          Transform3D* transform
      ) : AssetInstance(definition,transform) {
        return;
      }

      LightInstance::~LightInstance() {
        return;
      }

      bool LightInstance::load(std::string projectDir) {
        return false;
      }
    } // End of Graphics
  } // End of Components
} // End of Dream
