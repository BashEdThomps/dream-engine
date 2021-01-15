#include "AudioCache.h"


#include "Common/Constants.h"
#include "Common/Logger.h"

#include "AudioDefinition.h"
#include "Wav/WavAudioRuntime.h"
#include "Ogg/OggAudioRuntime.h"

namespace Dream
{

    AudioCache::AudioCache
    (ProjectRuntime* parent)
        : Cache(parent)
    {
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
        for (AssetRuntime* runt : mRuntimes)
        {
            delete runt;
        }
        mRuntimes.clear();
    }

    SharedAssetRuntime*
    AudioCache::loadRuntime
    (AssetDefinition* def)
    {
        auto aDef = static_cast<AudioDefinition*>(def);
        SharedAssetRuntime* asset = nullptr;

        if (def->getFormat() == Constants::ASSET_FORMAT_AUDIO_WAV)
        {
            asset = new WavAudioRuntime(aDef,mProjectRuntime);
        }
        else if (def->getFormat() == Constants::ASSET_FORMAT_AUDIO_OGG)
        {
            asset = new OggAudioRuntime(aDef,mProjectRuntime);
        }
        else
        {
            LOG_ERROR("AudioCache: Error, unrecognised audio format {}", def->getFormat());
        }

        if (asset)
        {
            asset->useDefinition();
            mRuntimes.push_back(asset);
        }

        return asset;
    }
}
