#include "TextureCache.h"
#include "TextureDefinition.h"
#include "TextureInstance.h"
#include "../../../deps/soil/SOIL.h"
#include "../../../Utilities/File.h"

namespace Dream
{
    TextureCache::TextureCache
    (ProjectRuntime* runtime)
        : ICache(runtime)
    {
        setLogClassName("TextureCache");
    }

    TextureCache::~TextureCache()
    {
        flushRawTextureImageData();
    }

    IAssetInstance*
    TextureCache::loadInstance
    (IAssetDefinition* def)
    {
        auto textureDef = dynamic_cast<TextureDefinition*>(def);
        auto log = getLog();
        string filename = getAbsolutePath(def);

        File txFile{filename};
        if (!txFile.exists())
        {
            log->error("Texture file does not exist: {}",filename);
            return nullptr;
        }

        log->debug("Loading texture: {}",filename);
        for (auto instance : mInstances)
        {
            auto nextTexture = dynamic_cast<TextureInstance*>(instance);
            if (nextTexture->getPath().compare(filename) == 0)
            {
                log->debug("Found cached texture by filename");
                return nextTexture;
            }
        }

        int width = 0;
        int height = 0;
        int channels = 0;

        unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);

        // Check image data against existing textures

        for (auto instance : mInstances)
        {
            auto nextTexture = dynamic_cast<TextureInstance*>(instance);
            if (nextTexture->getWidth() == width &&
                nextTexture->getHeight() == height &&
                nextTexture->getChannels() == channels)
            {
                log->debug("Found Similar Texture, comparing data");
                if (nextTexture->getImage() != nullptr)
                {
                    int compare = memcmp(nextTexture->getImage(), image, static_cast<size_t>(width*height*channels));
                    if (compare == 0)
                    {
                        log->debug("Found cached texture with data match for {}",filename);
                        SOIL_free_image_data(image);
                        return nextTexture;
                    }
                }
            }
        }

        log->debug("Didn't find cached texture matching {}",filename);
        log->debug("Loaded texture {} with width {}, height {}, channels {}",filename, width,height,channels);
        // Assign texture to ID
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        log->debug("Bound to texture id {}",textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

        glGenerateMipmap(GL_TEXTURE_2D);

        // Set Parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        checkGLError();

        auto texture = new TextureInstance(textureDef);
        texture->setPath(filename);
        texture->setGLID(textureID);
        texture->setWidth(width);
        texture->setHeight(height);
        texture->setChannels(channels);
        texture->setImage(image);
        mInstances.push_back(texture);
        return texture;
    }

    void
    TextureCache::flushRawTextureImageData
    ()
    {
       for (auto instance : mInstances)
       {
           auto t = dynamic_cast<TextureInstance*>(instance);
           if (t->getImage() != nullptr)
           {
               SOIL_free_image_data(t->getImage());
               t->setImage(nullptr);
           }
       }
    }

}

