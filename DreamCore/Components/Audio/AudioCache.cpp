#include "AudioCache.h"

#include "AudioDefinition.h"
#include "Wav/WavAudioRuntime.h"
#include "Ogg/OggAudioRuntime.h"

namespace Dream
{

    AudioCache::AudioCache
    (ProjectRuntime* parent)
        : Cache(parent)
    {
#ifdef DREAM_DEBUG
       setLogClassName("AudioCache");
#endif
    }

    AudioCache::~AudioCache
    ()
    {
        clear();
    }

    void
    AudioCache::clear
    ()
    {
        for (AssetRuntime* runt : mInstances)
        {
            delete runt;
        }
        mInstances.clear();
    }

    SharedAssetRuntime*
    AudioCache::loadInstance
    (AssetDefinition* def)
    {
#ifdef DREAM_DEBUG
        auto log = getLog();
#endif
        auto aDef = static_cast<AudioDefinition*>(def);
        SharedAssetRuntime* asset = nullptr;

        if (def->getFormat().compare(Constants::ASSET_FORMAT_AUDIO_WAV) == 0)
        {
            asset = new WavAudioRuntime(aDef,mProjectRuntime);
        }
        else if (def->getFormat().compare(Constants::ASSET_FORMAT_AUDIO_OGG) == 0)
        {
            asset = new OggAudioRuntime(aDef,mProjectRuntime);
        }
#ifdef DREAM_DEBUG
        else
        {
            log->error("Error, unrecognised audio format {}", def->getFormat());
        }
#endif

        if (asset)
        {
            asset->useDefinition();
            mInstances.push_back(asset);
        }

        return asset;
    }
}
