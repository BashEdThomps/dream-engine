#include "TextureCache.h"
#include "TextureDefinition.h"
#include "TextureRuntime.h"
#include "TextureTasks.h"
#include "../GraphicsComponent.h"
#include "../../SharedAssetRuntime.h"
#include "../../../Utilities/File.h"
#include "../../../Project/ProjectRuntime.h"
#include "../../../deps/soil/SOIL.h"

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
        flushRawTextureImageData();
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
        for (auto runtime : mRuntimes)
        {
            auto nextTexture = static_cast<TextureRuntime*>(runtime);
            if (nextTexture->getPath().compare(filename) == 0)
            {
                #ifdef DREAM_LOG
                getLog()->debug("Found cached texture by filename");
                #endif
                return nextTexture;
            }
        }

        int width = 0;
        int height = 0;
        int channels = 0;

        unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);

        // Check image data against existing textures

        for (auto runtime : mRuntimes)
        {
            auto nextTexture = static_cast<TextureRuntime*>(runtime);
            if (nextTexture->getWidth() == width &&
                nextTexture->getHeight() == height &&
                nextTexture->getChannels() == channels)
            {
                #ifdef DREAM_LOG
                getLog()->debug("Found Similar Texture, comparing data");
                #endif
                if (nextTexture->getImage() != nullptr)
                {
                    int compare = memcmp(nextTexture->getImage(), image, static_cast<size_t>(width*height*channels));
                    if (compare == 0)
                    {
                        #ifdef DREAM_LOG
                        getLog()->debug("Found cached texture with data match for {}",filename);
                        #endif
                        SOIL_free_image_data(image);
                        return nextTexture;
                    }
                }
            }
        }

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
        mRuntimes.push_back(texture);

        // Add Load Task
        mProjectRuntime->getGraphicsComponent()->pushTask(new TextureCreationTask(texture));
        return texture;
    }

    void
    TextureCache::flushRawTextureImageData
    ()
    {
       for (auto runtime : mRuntimes)
       {
           auto t = static_cast<TextureRuntime*>(runtime);
           if (t->getImage() != nullptr)
           {
               SOIL_free_image_data(t->getImage());
               t->setImage(nullptr);
           }
       }
    }

    void
    TextureCache::clear
    ()
    {
        flushRawTextureImageData();
        for (auto* runtime : mRuntimes)
        {
            delete runtime;
        }
        mRuntimes.clear();
    }
}

