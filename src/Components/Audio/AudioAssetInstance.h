#ifndef AudioAssetInstance_h
#define AudioAssetInstance_h

#include <vector>
#include "../../AssetInstance.h"

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

// 32 KB buffers
#define BUFFER_SIZE 32768

namespace Dream {
  namespace Components {
    namespace Audio {

      enum AudioAssetStatus {
        PLAYING,
        PAUSED,
        STOPPED,
        UNKNOWN
      };

      class AudioAssetInstance : public AssetInstance {
      protected:
        bool              mLooping;
        ALenum            mFormat;
        ALsizei           mFrequency;
        std::vector<char> mAudioDataBuffer;
        ALuint            mSource;
        ALuint            mBuffer;
      public:
        AudioAssetInstance(AssetDefinition*,Transform3D*);

        void setLooping(bool);
        bool isLooping();

        std::vector<char> getAudioDataBuffer();
        ALuint getFrequency();
        ALenum getFormat();

        void   setBuffer(ALuint buffer);
        ALuint getBuffer();

        void setSource(ALuint source);
        ALuint getSource();
      };

    } // End of Audio
  } // End of Components
} // End of Dream

#endif /* AudioAssetInstance_h */
