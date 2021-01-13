#pragma once

#include "Components/Cache.h"

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
    protected:
        SharedAssetRuntime* loadRuntime(AssetDefinition* def) override;
    };
}
