#ifndef AudioAssetInstance_h
#define AudioAssetInstance_h

#include <vector>
#include "AudioStatus.h"
#include "../../IAssetInstance.h"

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include </al.h>
#include </alc.h>
#endif

// 32 KB buffers
#define BUFFER_SIZE 32768

namespace Dream
{
      class AudioInstance : public IAssetInstance
      {
      protected:
        bool              mLooping;
        ALenum            mFormat;
        ALsizei           mFrequency;
        std::vector<char> mAudioDataBuffer;
        ALuint            mSource;
        ALuint            mBuffer;
        AudioStatus       mStatus;
      public:
        AudioInstance(AssetDefinition*,Transform3D*);

        void setLooping(bool);
        bool isLooping();

        std::vector<ALchar> getAudioDataBuffer();
        ALsizei getFrequency();
        ALenum  getFormat();
        AudioStatus getStatus();
        void setStatus(AudioStatus);
        void   setBuffer(ALuint buffer);
        ALuint getBuffer();

        void setSource(ALuint source);
        ALuint getSource();
      };

} // End of Dream

#endif /* AudioAssetInstance_h */
