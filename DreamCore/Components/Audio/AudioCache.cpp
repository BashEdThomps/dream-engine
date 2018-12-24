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
       setLogClassName("AudioCache");
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
        auto log = getLog();
        auto aDef = dynamic_cast<AudioDefinition*>(def);
        SharedAssetRuntime* asset = nullptr;

        if (def->getFormat().compare(Constants::ASSET_FORMAT_AUDIO_WAV) == 0)
        {
            asset = new WavAudioRuntime(aDef,mProjectRuntime);
            asset->useDefinition();
        }
        else if (def->getFormat().compare(Constants::ASSET_FORMAT_AUDIO_OGG) == 0)
        {
            asset = new OggAudioRuntime(aDef,mProjectRuntime);
            asset->useDefinition();
        }
        else
        {
            log->error("Error, unrecognised audio format {}", def->getFormat());
        }

        return asset;
    }
}
