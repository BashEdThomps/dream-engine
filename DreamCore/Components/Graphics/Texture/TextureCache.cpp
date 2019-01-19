#include "TextureCache.h"
#include "TextureDefinition.h"
#include "TextureRuntime.h"
#include "../../../deps/soil/SOIL.h"
#include "../../../Utilities/File.h"
#include "../../SharedAssetRuntime.h"

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
        // Assign texture to ID
        GLuint textureID;

        #ifdef DREAM_LOG
        checkGLError();
        #endif

        glGenTextures(1, &textureID);
        #ifdef DREAM_LOG
        checkGLError();
        #endif

        glBindTexture(GL_TEXTURE_2D, textureID);
        #ifdef DREAM_LOG
        checkGLError();
        getLog()->debug("Bound to texture id {}",textureID);
        #endif

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        #ifdef DREAM_LOG
        checkGLError();
        #endif

        glGenerateMipmap(GL_TEXTURE_2D);
        #ifdef DREAM_LOG
        checkGLError();
        #endif

        // Set Parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        #ifdef DREAM_LOG
        checkGLError();
        #endif

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        #ifdef DREAM_LOG
        checkGLError();
        #endif

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        #ifdef DREAM_LOG
        checkGLError();
        #endif
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        #ifdef DREAM_LOG
        checkGLError();
        #endif

        auto texture = new TextureRuntime(textureDef,mProjectRuntime);
        texture->setPath(filename);
        texture->setGLID(textureID);
        texture->setWidth(width);
        texture->setHeight(height);
        texture->setChannels(channels);
        texture->setImage(image);
        mRuntimes.push_back(texture);
        return texture;
    }

    void
    TextureCache::flushRawTextureImageData
    ()
    {
       for (auto Runtime : mRuntimes)
       {
           auto t = static_cast<TextureRuntime*>(Runtime);
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
        for (auto* Runtime : mRuntimes)
        {
            delete Runtime;
        }
        mRuntimes.clear();
    }
}

