#ifndef AudioInstance_h
#define AudioInstance_h

#include <vector>
#include <SDL2/SDL_mixer.h>
#include "../AudioStatus.h"
#include "../IAudioInstance.h"
#include "../../../Constants.h"

namespace Dream {

    class SDLAudioInstance : public IAudioInstance {
    protected:
      Mix_Chunk *mChunk;
      Mix_Music *mMusic;
      int mChannel;
    public:
      SDLAudioInstance(AssetDefinition*,Transform3D*);
      ~SDLAudioInstance();
      bool load(string);
      void loadExtraAttributes(nlohmann::json);

      AudioStatus getStatus();
      bool play();
      bool pause();
      bool stop();
    }; // End of SDLAudioInstance
} // End of Dream

#endif // AudioInstance_h
