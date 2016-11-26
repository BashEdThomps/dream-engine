#ifndef LightInstance_h
#define LightInstance_h

#include "../../AssetDefinition.h"
#include "../../AssetInstance.h"

namespace Dream {
      class LightInstance : public AssetInstance {
      public:
        LightInstance(AssetDefinition*,Transform3D*);
        ~LightInstance();
        bool load(string);
        void loadExtraAttributes(nlohmann::json) {}
      }; // End of LightInstance
} // End of Dream

#endif /* LightInstance_h */
