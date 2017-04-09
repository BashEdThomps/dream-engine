#ifndef LightInstance_h
#define LightInstance_h

#include "../../Constants.h"
#include "../../AssetDefinition.h"
#include "../../AssetInstance.h"

namespace Dream {
      class LightInstance : public AssetInstance {
          glm::vec3 mColor;
          float mIntensity;
      public:
        LightInstance(AssetDefinition*,Transform3D*);
        ~LightInstance();
        bool load(string);
        void loadExtraAttributes(nlohmann::json);

        glm::vec3 getColor();
        float getIntensity();
      }; // End of LightInstance
} // End of Dream

#endif /* LightInstance_h */
