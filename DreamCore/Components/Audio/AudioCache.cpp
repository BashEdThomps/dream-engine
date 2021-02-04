#include "AudioCache.h"


#include "Common/Constants.h"
#include "Common/Logger.h"
#include "Project/ProjectRuntime.h"

#include "AudioDefinition.h"
#include "AudioComponent.h"
#include "AudioRuntime.h"

using std::lock_guard;

namespace octronic::dream
{

    AudioCache::AudioCache
    (ProjectRuntime* parent)
        : Cache("AudioCache",parent)
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
        if (dreamTryLock())
        {
            dreamLock();
            for (AssetRuntime* runt : mRuntimes)
            {
                delete runt;
            }
            mRuntimes.clear();
        } dreamElseLockFailed
    }

    SharedAssetRuntime*
    AudioCache::loadRuntime
    (AssetDefinition* def)
    {
        if (dreamTryLock())
        {
            dreamLock();

            auto aDef = static_cast<AudioDefinition*>(def);
            AudioComponent* ac = mProjectRuntime->getAudioComponent();
            AudioRuntime* asset = ac->newAudioRuntime(aDef);

            if (asset)
            {
                if (!asset->useDefinition())
                {
                    delete asset;
                    asset = nullptr;
                }
                else
                {
                    mRuntimes.push_back(asset);
                }
            }
            return asset;
        } dreamElseLockFailed
    }
}
