#include "TextureCache.h"
#include "TextureDefinition.h"
#include "TextureRuntime.h"
#include "TextureTasks.h"
#include "../GraphicsComponent.h"
#include "../../SharedAssetRuntime.h"
#include "../../../Common/File.h"
#include "../../../Project/ProjectRuntime.h"
#include "soil/SOIL.h"

namespace Dream
{
    TextureCache::TextureCache
    (ProjectRuntime* runtime)
        : Cache(runtime)
    {
        #ifdef DREAM_LOG
        setLogClassName("TextureCache");
        #endif
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
            #ifdef DREAM_LOG
            getLog()->error("Cannot load texture, TextureDefinition is null");
            #endif
            return nullptr;
        }
        auto textureDef = static_cast<TextureDefinition*>(def);
        string filename = getAbsolutePath(def);

        File txFile(filename);
        if (!txFile.exists())
        {
            #ifdef DREAM_LOG
            getLog()->error("Texture file does not exist: {}",filename);
            #endif
            return nullptr;
        }

        #ifdef DREAM_LOG
        getLog()->debug("Loading texture: {}",filename);
        #endif

        int width = 0;
        int height = 0;
        int channels = 0;

        unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);

        #ifdef DREAM_LOG
        getLog()->debug("Didn't find cached texture matching {}",filename);
        getLog()->debug("Loaded texture {} with width {}, height {}, channels {}",filename, width,height,channels);
        #endif

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

