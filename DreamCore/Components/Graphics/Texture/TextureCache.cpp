#include "TextureCache.h"

#include "TextureDefinition.h"
#include "TextureRuntime.h"
#include "TextureTasks.h"
#include "Components/Storage/File.h"
#include "Components/Storage/StorageManager.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Components/SharedAssetRuntime.h"
#include "Project/ProjectRuntime.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace octronic::dream
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

        StorageManager* fm = mProjectRuntime->getStorageManager();
        File* txFile = fm->openFile(filename);
        if (!txFile->exists())
        {
            LOG_ERROR("TextureCache: Texture file does not exist: {}",filename);
            fm->closeFile(txFile);
            txFile = nullptr;
            return nullptr;
        }

        LOG_DEBUG("TextureCache: Loading texture: {}",filename);

        if (!txFile->readBinary())
        {
            LOG_ERROR("TextureCache: Unable to read file data");
            fm->closeFile(txFile);
            txFile = nullptr;
            return nullptr;
        }
        uint8_t* buffer = txFile->getBinaryData();
        size_t buffer_sz = txFile->getBinaryDataSize();

        int width = 0;
        int height = 0;
        int channels = 0;

	    stbi_set_flip_vertically_on_load(true);
		uint8_t* image = stbi_load_from_memory(
			static_cast<const stbi_uc*>(buffer),
			buffer_sz,
			&width, &height,
			&channels, 0
		);

        fm->closeFile(txFile);
        txFile = nullptr;

        LOG_DEBUG(
    		"TextureCache: Loaded texture {} with width {}, height {}, channels {}",
            filename, width,height,channels);

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

