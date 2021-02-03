#include "TextureCache.h"

#include "TextureDefinition.h"
#include "TextureRuntime.h"
#include "TextureTasks.h"
#include "Components/Storage/File.h"
#include "Components/Storage/StorageManager.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Components/SharedAssetRuntime.h"
#include "Project/ProjectRuntime.h"

using std::unique_lock;

namespace octronic::dream
{
    TextureCache::TextureCache
    (ProjectRuntime* runtime)
        : Cache("TextureCache",runtime)
    {
    }

    TextureCache::~TextureCache()
    {
    }

    SharedAssetRuntime*
    TextureCache::loadRuntime
    (AssetDefinition* def)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (!def)
        {
            LOG_ERROR("TextureCache: Cannot load texture, TextureDefinition is null");
            return nullptr;
        }
        auto textureDef = static_cast<TextureDefinition*>(def);
        auto texture = new TextureRuntime(textureDef,mProjectRuntime);



        if (!texture->useDefinition())
        {
           delete texture;
            texture = nullptr;
        }
        else
        {
        	mRuntimes.push_back(texture);
        }

        return texture;
    }

    void
    TextureCache::clear
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        for (auto* runtime : mRuntimes)
        {
            delete runtime;
        }
        mRuntimes.clear();
    }
}

