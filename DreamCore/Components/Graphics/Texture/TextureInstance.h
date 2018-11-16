/*
 * TextureInstance
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

#pragma once

#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#endif

#include <iostream>
//#include <assimp/types.h>
//#include <assimp/material.h>
#include "../../IAssetInstance.h"

using namespace std;


namespace Dream
{
    class TextureDefinition;
    class SceneObjectRuntime;

    class TextureInstance : public IAssetInstance
    {
    private:
        GLuint mGLID = 0;
        string mPath = "";
        int mWidth = 0;
        int mHeight = 0;
        int mChannels = 0;
        unsigned char* mImage = nullptr;
    public:
        TextureInstance(TextureDefinition* def);
        ~TextureInstance() override;

        bool operator==(const TextureInstance& other);

        bool load(string) override;

        GLuint getGLID() const;
        void setGLID(const GLuint& gLID);
        string getPath() const;
        void setPath(const string& path);
        int getWidth() const;
        void setWidth(int width);
        int getHeight() const;
        void setHeight(int height);
        int getChannels() const;
        void setChannels(int channels);
        unsigned char* getImage() const;
        void setImage(unsigned char* image);
    };
} // end of Dream
