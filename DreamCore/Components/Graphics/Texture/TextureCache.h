#pragma once

#include "../../ICache.h"

namespace Dream
{
    class TextureInstance;
    class TextureDefinition;

    class TextureCache : public ICache
    {
    public:
        TextureCache(ProjectRuntime* runtime);
        ~TextureCache() override;
        void flushRawTextureImageData();

    protected:
        IAssetInstance* loadInstance(IAssetDefinition* def) override;
    };
}
