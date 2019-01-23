#pragma once

#include "../../Cache.h"

namespace Dream
{
    class TextureRuntime;
    class TextureDefinition;
    class GraphicsComponent;

    class TextureCache : public Cache
    {
    public:
        TextureCache(ProjectRuntime* runtime);
        ~TextureCache() override;
        void clear() override;
        void flushRawTextureImageData();
    protected:
        SharedAssetRuntime* loadRuntime(AssetDefinition* def) override;
    };
}
