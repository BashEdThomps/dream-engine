#ifndef LightInstance_h
#define LightInstance_h

#include "../../AssetDefinition.h"
#include "../../AssetInstance.h"

namespace Dream {
  namespace Components {
    namespace Video {
      class LightInstance : public AssetInstance {
      public:
        LightInstance(AssetDefinition*,Transform3D*);
        ~LightInstance();
        bool load(std::string);
        void loadExtraAttributes(nlohmann::json) {}
      }; // End of LightInstance
    } // End of Video
  } // End of Components
} // End of Dream

#endif /* LightInstance_h */
