#pragma once

#include "../../Cache.h"

namespace Dream
{
    class TextureInstance;
    class TextureDefinition;

    class TextureCache : public Cache
    {
    public:
        TextureCache(ProjectRuntime* runtime);
        ~TextureCache() override;
        void clear() override;
        void flushRawTextureImageData();
    protected:
        SharedAssetInstance* loadInstance(AssetDefinition* def) override;
    };
}
