#include "TextureRuntime.h"
#include "TextureDefinition.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Components/Storage/File.h"
#include "Components/Storage/StorageManager.h"
#include "Components/Storage/ProjectDirectory.h"
#include "Scene/Entity/EntityRuntime.h"
#include "Project/Project.h"
#include "Project/ProjectRuntime.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using std::make_shared;


namespace octronic::dream
{
    TextureRuntime::TextureRuntime
    (TextureDefinition* def, ProjectRuntime* rt)
        : SharedAssetRuntime ("TextureRuntime",def,rt),
          mGLID(0),
          mWidth(0),
          mHeight(0),
          mChannels(0),
          mImage(nullptr),
          mTextureConstructionTask(this),
          mTextureDestructionTask(nullptr)
    {}

    TextureRuntime::~TextureRuntime
    ()
    {
        mTextureDestructionTask = make_shared<TextureDestructionTask>();
        mTextureDestructionTask->setGLID(mGLID);
        mProjectRuntime->getGraphicsComponent()->pushDestructionTask(mTextureDestructionTask);
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
        if(dreamTryLock()) {
            dreamLock();
            mWidth = width;
        } dreamElseLockFailed
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
        if(dreamTryLock()) {
            dreamLock();
            mHeight = height;
        } dreamElseLockFailed
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
        if(dreamTryLock()) {
            dreamLock();
            mChannels = channels;
        } dreamElseLockFailed
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
        if(dreamTryLock()) {
            dreamLock();
            mImage = image;
        } dreamElseLockFailed
    }

    void TextureRuntime::pushConstructionTask()
    {
        if(dreamTryLock()) {
            dreamLock();
            mProjectRuntime->getGraphicsComponent()->pushTask(&mTextureConstructionTask);
        } dreamElseLockFailed
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
        if(dreamTryLock()) {
            dreamLock();
            mGLID = gLID;
        } dreamElseLockFailed
    }

    bool
    TextureRuntime::operator==
    (const TextureRuntime& other)
    {
        return this->mGLID == other.mGLID &&
                this->mUuid == other.mUuid;
    }

    bool
    TextureRuntime::useDefinition
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            LOG_TRACE("TextureRuntime: {}",__FUNCTION__);

            // All aboard
            string filename = mProjectRuntime
                    ->getProject()
                    ->getDirectory()
                    ->getAssetAbsolutePath(mDefinition->getUuid());

            StorageManager* fm = mProjectRuntime->getStorageManager();
            File* txFile = fm->openFile(filename);
            if (!txFile->exists())
            {
                LOG_ERROR("TextureCache: Texture file does not exist: {}",filename);
                fm->closeFile(txFile);
                txFile = nullptr;
                return false;
            }

            LOG_DEBUG("TextureCache: Loading texture: {}",filename);

            if (!txFile->readBinary())
            {
                LOG_ERROR("TextureCache: Unable to read file data");
                fm->closeFile(txFile);
                txFile = nullptr;
                return false;
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

            //setPath(filename);
            setWidth(width);
            setHeight(height);
            setChannels(channels);
            setImage(image);
            pushConstructionTask();
            return true;
        } dreamElseLockFailed
    }

    bool TextureRuntime::loadIntoGL()
    {
        if(dreamTryLock()) {
            dreamLock();
            LOG_TRACE("TextureRuntime: {}",__FUNCTION__);
            // Assign texture to ID
            GLuint textureID;

            GLCheckError();

            glGenTextures(1, &textureID);
            GLCheckError();

            glBindTexture(GL_TEXTURE_2D, textureID);
            GLCheckError();
            LOG_DEBUG("TextureRuntime: Bound to texture id {}",textureID);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, getWidth(), getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,getImage());
            GLCheckError();

            glGenerateMipmap(GL_TEXTURE_2D);
            GLCheckError();

            // Set Parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            GLCheckError();

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            GLCheckError();

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
            GLCheckError();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            setGLID(textureID);

            glBindTexture(GL_TEXTURE_2D, 0);

            GLCheckError();
            delete getImage();
            setImage(nullptr);

            return true;
        } dreamElseLockFailed
    }
}
