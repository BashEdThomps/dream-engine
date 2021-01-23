#pragma once

#include "Components/Cache.h"

namespace octronic::dream
{
    class AudioCache : public Cache
    {
    public:
        AudioCache(ProjectRuntime* parent);
        ~AudioCache() override;
        void clear() override;

    protected:
        SharedAssetRuntime* loadRuntime(AssetDefinition* definition) override;
    };
}
