/*
 * TextureCache.cpp
 *
 * Created: 30/11/2016 2016 by Ashley
 *
 * Copyright 2016 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */
#include "MaterialCache.h"

namespace Dream
{
    MaterialCache::MaterialCache
    () : DreamObject ("MaterialCache")
    {
        auto log = getLog();
        log->trace( "Constructing" );
    }

    MaterialCache::~MaterialCache
    ()
    {
        auto log = getLog();
        log->trace( "Destructing" );

        flushRawTextureImageData();

        for (Texture* texture : mTextureCache)
        {
            glDeleteTextures(1,&texture->id);
            checkGLError();
            delete texture;
        }
        mTextureCache.clear();

        for (AssimpMaterial* mat : mMaterialCache)
        {
            delete mat;
        }
        mMaterialCache.clear();
        return;
    }

    vector<Texture*>&
    MaterialCache::getTextureCache
    ()
    {
        return mTextureCache;
    }

    vector<AssimpMaterial*>&
    MaterialCache::getMaterialCache
    ()
    {
        return mMaterialCache;
    }

    AssimpMaterial*
    MaterialCache::getMaterialByName
    (aiString name)
    {
        auto log = getLog();
        for (AssimpMaterial* material : mMaterialCache)
        {
            if (material->mName == name)
            {
                log->info("Found existing material {}", name.C_Str());
                return material;
            }
        }
        return nullptr;
    }

    void MaterialCache::addMaterialToCache(AssimpMaterial* mat)
    {
        auto log = getLog();
        log->info("Adding material to map {}", mat->mName.C_Str());
        mMaterialCache.push_back(mat);
    }

    Texture*
    MaterialCache::loadTextureFromFile
    (const char* file_c, const char* directory_c, const char* type)
    {
        auto log = getLog();
        log->info( "Loading from: {}/{}", directory_c , file_c );
        //Generate texture ID and load texture data
        string filename = string(file_c);
        string directory = string(directory_c);
        filename = directory + '/' + filename;

        for (Texture* nextTexture : mTextureCache)
        {
            if (nextTexture->path == filename && nextTexture->type == type)
            {
                log->info("Found cached texture by filename");
                return nextTexture;
            }
        }

        GLuint textureID;
        glGenTextures(1, &textureID);

        int width = 0;
        int height = 0;
        int channels = 0;

        unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);

        // Check image data against existing textures

        for (Texture* nextTexture : mTextureCache)
        {
            if (nextTexture->width == width &&
                nextTexture->height == height &&
                nextTexture->channels == channels)
            {
                log->info("Found Similar Texture, comparing data");
                int compare = memcmp(nextTexture->image, image, static_cast<size_t>(width*height*channels));
                if (compare == 0)
                {
                    log->info("Found cached texture with data match for {}",filename);
                    SOIL_free_image_data(image);
                    return nextTexture;
                }
            }
        }
        log->info("Didn't find cached texture matching {}",filename);

        log->info("Loaded texture {} with width {}, height {}, channels {}",filename, width,height,channels);
        // Assign texture to ID
        glBindTexture(GL_TEXTURE_2D, textureID);
        log->info("Bound to texture id {}",textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

        glGenerateMipmap(GL_TEXTURE_2D);

        // Set Parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        checkGLError();

        auto texture = new Texture();
        texture->path = filename;
        texture->id = textureID;
        texture->type = type;
        texture->width = width;
        texture->height = height;
        texture->channels = channels;
        texture->image = image;
        mTextureCache.push_back(texture);
        return texture;
    }

    AssimpMaterial*MaterialCache::newAssimpMaterial()
    {
        auto newAM = new AssimpMaterial();
        addMaterialToCache(newAM);
        return newAM;
    }

    void MaterialCache::flushRawTextureImageData()
    {
       for (Texture* t : mTextureCache)
       {
        SOIL_free_image_data(t->image);
       }
    }

} // End of Dream
