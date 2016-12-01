#ifndef AudioInstance_h
#define AudioInstance_h

#include <vector>
#include <SDL2/SDL_mixer.h>
#include "AudioStatus.h"
#include "../../AssetInstance.h"

namespace Dream {

    class AudioInstance : public AssetInstance {
    protected:
      bool mLooping;
      AudioStatus mStatus;
      Mix_Chunk *mChunk;
      Mix_Music *mMusic;
    public:
      AudioInstance(AssetDefinition*,Transform3D*);
      ~AudioInstance();
      bool load(string);
      void loadExtraAttributes(nlohmann::json);
      void setLooping(bool);
      bool isLooping();
      void setStatus(AudioStatus);
      AudioStatus getStatus();

      bool play();
      bool pause();
      bool stop();

    };

} // End of Dream

#endif // AudioInstance_h
