/*
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
    #include <GL/glew.h>
    #include <GL/glu.h>
#endif

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include <GL/glew.h>
    #include <OpenGL/gl3.h>
#endif

#ifdef __linux__
    #include <GL/glew.h>
    #include <GL/glu.h>
#endif

#include <iostream>
#include "../../SharedAssetRuntime.h"
#include "../GraphicsComponentTask.h"

using namespace std;


namespace Dream
{
    class TextureDefinition;
    class SceneObjectRuntime;
    class Runtime;



    class TextureRuntime : public SharedAssetRuntime
    {
    private:
        GLuint mGLID = 0;
        string mPath = "";
        int mWidth = 0;
        int mHeight = 0;
        int mChannels = 0;
        unsigned char* mImage = nullptr;
        GraphicsComponentTask* mCreateTextureTask;

    public:
        TextureRuntime(TextureDefinition* def, ProjectRuntime*);
        ~TextureRuntime() override;

        bool operator==(const TextureRuntime& other);

        bool useDefinition() override;

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

        void clearCreateTextureTask();
    };


} // end of Dream
