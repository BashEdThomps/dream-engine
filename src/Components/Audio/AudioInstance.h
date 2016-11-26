#ifndef AudioAssetInstance_h
#define AudioAssetInstance_h

#include <vector>
#include "../../AssetInstance.h"
#include "AudioStatus.h"

namespace Dream {

      class AudioInstance : public AssetInstance {
      protected:
        bool mLooping;
        AudioStatus mStatus;
      public:
        AudioInstance(AssetDefinition*,Transform3D*);
        ~AudioInstance();
        bool load(string);
        void loadExtraAttributes(nlohmann::json);
        void setLooping(bool);
        bool isLooping();
        void setStatus(AudioStatus);
        AudioStatus getStatus();
      };

} // End of Dream

#endif /* AudioAssetInstance_h */
