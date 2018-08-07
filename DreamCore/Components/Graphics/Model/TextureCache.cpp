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
#include "TextureCache.h"

namespace Dream
{
    TextureCache::TextureCache
    () : ILoggable ("TextureCache")
    {
        auto log = getLog();
            log->info( "TextureCache: Constructing" );
    }

    TextureCache::~TextureCache
    ()
    {
        auto log = getLog();
            log->info( "TextureCache: Constructing" );

        for (Texture texture : mCache)
        {
            glDeleteTextures(1,&texture.id);
            Constants::checkGLError("After free texture");
        }
        return;
    }

    vector<Texture>
    TextureCache::getTextureCache
    ()
    {
        return mCache;
    }

    Texture
    TextureCache::loadTextureFromFile
    (const char* file_c, const char* directory_c, const char* type)
    {
        auto log = getLog();
            log->info( "TextureCache: Loading from: {}/{}", directory_c , file_c );
        //Generate texture ID and load texture data
        string filename = string(file_c);
        string directory = string(directory_c);
        filename = directory + '/' + filename;

        for (Texture nextTexture : mCache)
        {
            if (nextTexture.path == filename && nextTexture.type == type)
            {
                    log->info( "TextureCache: Found cached texture." );
                return nextTexture;
            }
        }

        GLuint textureID;
        glGenTextures(1, &textureID);

        int width = 0;
        int height = 0;
        int channels = 0;

        unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
        log->info("Loaded texture {} with width {}, height {}, channels {}",filename, width,height,channels);
        // Assign texture to ID
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        Constants::checkGLError("After texture creation");

        SOIL_free_image_data(image);

        Texture texture;
        texture.path = filename;
        texture.id = textureID;
        texture.type = type;
        texture.width = width;
        texture.height = height;
        mCache.push_back(texture);

        return texture;
    }

} // End of Dream
