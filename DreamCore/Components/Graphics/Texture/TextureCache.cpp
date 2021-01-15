#include "TextureCache.h"

#include "TextureDefinition.h"
#include "TextureRuntime.h"
#include "TextureTasks.h"
#include "Common/File.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Components/SharedAssetRuntime.h"
#include "Project/ProjectRuntime.h"

#include <SOIL.h>

namespace Dream
{
    TextureCache::TextureCache
    (ProjectRuntime* runtime)
        : Cache(runtime)
    {
    }

    TextureCache::~TextureCache()
    {
    }

    SharedAssetRuntime*
    TextureCache::loadRuntime
    (AssetDefinition* def)
    {
        if (!def)
        {
            LOG_ERROR("TextureCache: Cannot load texture, TextureDefinition is null");
            return nullptr;
        }
        auto textureDef = static_cast<TextureDefinition*>(def);
        string filename = getAbsolutePath(def);

        File txFile(filename);
        if (!txFile.exists())
        {
            LOG_ERROR("TextureCache: Texture file does not exist: {}",filename);
            return nullptr;
        }

        LOG_DEBUG("TextureCache: Loading texture: {}",filename);

        int width = 0;
        int height = 0;
        int channels = 0;

        unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);

        LOG_DEBUG("TextureCache: Didn't find cached texture matching {}",filename);
        LOG_DEBUG("TextureCache: Loaded texture {} with width {}, height {}, channels {}",filename, width,height,channels);

        auto texture = new TextureRuntime(textureDef,mProjectRuntime);
        texture->setPath(filename);
        texture->setWidth(width);
        texture->setHeight(height);
        texture->setChannels(channels);
        texture->setImage(image);
        texture->pushConstructionTask();
        mRuntimes.push_back(texture);

        // Add Load Task
        return texture;
    }

    void
    TextureCache::clear
    ()
    {
        for (auto* runtime : mRuntimes)
        {
            delete runtime;
        }
        mRuntimes.clear();
    }
}

