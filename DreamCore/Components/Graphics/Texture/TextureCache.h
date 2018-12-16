#pragma once

#include "../../Cache.h"

namespace Dream
{
    class TextureRuntime;
    class TextureDefinition;

    class TextureCache : public Cache
    {
    public:
        TextureCache(ProjectRuntime* runtime);
        ~TextureCache() override;
        void clear() override;
        void flushRawTextureImageData();
    protected:
        SharedAssetRuntime* loadInstance(AssetDefinition* def) override;
    };
}
