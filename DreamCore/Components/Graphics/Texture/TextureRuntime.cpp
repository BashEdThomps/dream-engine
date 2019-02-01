#include "TextureRuntime.h"
#include "TextureDefinition.h"
#include "TextureTasks.h"
#include "../GraphicsComponent.h"
#include "../../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../../Project/ProjectRuntime.h"

namespace Dream
{
    TextureRuntime::TextureRuntime
    (TextureDefinition* def, ProjectRuntime* rt)
        : SharedAssetRuntime (def,rt),
          mGLID(0),
          mPath(""),
          mWidth(0),
          mHeight(0),
          mChannels(0),
          mImage(nullptr),
          mCreateTextureTask(nullptr)
    {}

    TextureRuntime::~TextureRuntime
    ()
    {
        if (mCreateTextureTask)
        {
            mCreateTextureTask->setExpired(true);
        }

        mProjectRuntime->getGraphicsComponent()->pushTask(new TextureDeletionTask(mGLID));
        #ifdef DREAM_LOG
        checkGLError();
        #endif
    }

    string
    TextureRuntime::getPath
    () const
    {
        return mPath;
    }

    void
    TextureRuntime::setPath
    (const string& path)
    {
        mPath = path;
    }

    int
    TextureRuntime::getWidth
    () const
    {
        return mWidth;
    }

    void
    TextureRuntime::setWidth
    (int width)
    {
        mWidth = width;
    }

    int
    TextureRuntime::getHeight
    () const
    {
        return mHeight;
    }

    void
    TextureRuntime::setHeight
    (int height)
    {
        mHeight = height;
    }

    int
    TextureRuntime::getChannels
    () const
    {
        return mChannels;
    }

    void
    TextureRuntime::setChannels
    (int channels)
    {
        mChannels = channels;
    }

    unsigned char*
    TextureRuntime::getImage
    () const
    {
        return mImage;
    }

    void
    TextureRuntime::setImage
    (unsigned char* image)
    {
        mImage = image;
    }

    void
    TextureRuntime::clearCreateTextureTask
    ()
    {
        mCreateTextureTask = nullptr;
    }

    void
    TextureRuntime::setTextureCreationTask
    (TextureCreationTask* task)
    {
       mCreateTextureTask = task;
    }

    GLuint
    TextureRuntime::getGLID
    () const
    {
        return mGLID;
    }

    void
    TextureRuntime::setGLID
    (const GLuint& gLID)
    {
        mGLID = gLID;
    }

    bool
    TextureRuntime::operator==
    (const TextureRuntime& other)
    {
        return this->mGLID == other.mGLID && this->mPath.compare(other.mPath) == 0;
    }

    bool
    TextureRuntime::useDefinition
    ()
    {
        return false;
    }
}
