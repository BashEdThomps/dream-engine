#ifndef AudioInstance_h
#define AudioInstance_h

#include <vector>
#include <SDL2/SDL_mixer.h>
#include "AudioStatus.h"
#include "../../AssetInstance.h"
#include "../../Constants.h"

namespace Dream {

    class AudioInstance : public AssetInstance {
    protected:
      bool mLooping;
      Mix_Chunk *mChunk;
      Mix_Music *mMusic;
      int mChannel;
    public:
      AudioInstance(AssetDefinition*,Transform3D*);
      ~AudioInstance();
      bool load(string);
      void loadExtraAttributes(nlohmann::json);
      void setLooping(bool);
      bool isLooping();

      AudioStatus getStatus();

      bool play();
      bool pause();
      bool stop();

    };

} // End of Dream

#endif // AudioInstance_h
