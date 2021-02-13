#include "TextureRuntime.h"
#include "TextureDefinition.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Storage/File.h"
#include "Storage/StorageManager.h"
#include "Storage/ProjectDirectory.h"
#include "Scene/SceneRuntime.h"
#include "Entity/EntityRuntime.h"
#include "Project/Project.h"
#include "Project/ProjectRuntime.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using std::make_shared;


namespace octronic::dream
{
    TextureRuntime::TextureRuntime
    (ProjectRuntime* rt, TextureDefinition* def)
        : SharedAssetRuntime(rt, def),
          mGLID(0),
          mWidth(0),
          mHeight(0),
          mChannels(0),
          mImage(nullptr),
          mTextureLoadIntoGLTask(make_shared<TextureLoadIntoGLTask>(rt, this)),
          mTextureRemoveFromGLTask(make_shared<TextureRemoveFromGLTask>(rt))
    {}

    TextureRuntime::~TextureRuntime
    ()
    {
        mTextureRemoveFromGLTask->setGLID(mGLID);
        auto gfxDestructionQueue = mProjectRuntimeHandle->getGraphicsComponent()->getDestructionTaskQueue();
        gfxDestructionQueue->pushTask(mTextureRemoveFromGLTask);
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
        return this->mGLID == other.mGLID && this->mUuid == other.mUuid;
    }

    bool
    TextureRuntime::loadFromDefinition
    ()
    {
        LOG_TRACE("TextureRuntime: {}",__FUNCTION__);

        // All aboard
        string filename = mProjectRuntimeHandle
                ->getProject()
                ->getDirectory()
                ->getAssetAbsolutePath(mDefinitionHandle->getUuid());

        StorageManager* fm = mProjectRuntimeHandle->getStorageManager();
        File* txFile = fm->openFile(filename);

        if (!txFile->exists())
        {
            LOG_ERROR("TextureRuntime: Texture file does not exist: {}",filename);
            fm->closeFile(txFile);
            txFile = nullptr;
            mLoadError = true;
            return false;
        }

        LOG_DEBUG("TextureRuntime: Loading texture: {}",filename);

        if (!txFile->readBinary())
        {
            LOG_ERROR("TextureRuntime: Unable to read file data");
            fm->closeFile(txFile);
            txFile = nullptr;
            mLoadError = true;
            return false;
        }

        uint8_t* buffer = txFile->getBinaryData();
        size_t buffer_sz = txFile->getBinaryDataSize();

        int width = 0;
        int height = 0;
        int channels = 0;

        stbi_set_flip_vertically_on_load(true);
        uint8_t* image = stbi_load_from_memory(static_cast<const stbi_uc*>(buffer),
                                               buffer_sz, &width, &height, &channels, 0);

        fm->closeFile(txFile);
        txFile = nullptr;

        LOG_DEBUG(
                    "TextureRuntime: Loaded texture {} with width {}, height {}, channels {}",
                    filename, width,height,channels);

        //setPath(filename);
        setWidth(width);
        setHeight(height);
        setChannels(channels);
        setImage(image);
        return true;
    }

    bool TextureRuntime::loadIntoGL()
    {
        LOG_TRACE("TextureRuntime: {}",__FUNCTION__);
        // Assign texture to ID

        glGenTextures(1, &mGLID);
        GLCheckError();

        glBindTexture(GL_TEXTURE_2D, mGLID);
        GLCheckError();
        LOG_DEBUG("TextureRuntime: Bound to texture id {}",mGLID);

        if (mChannels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, getWidth(), getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,getImage());
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, getWidth(), getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,getImage());
        }
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

        glBindTexture(GL_TEXTURE_2D, 0);
        GLCheckError();

        delete getImage();
        setImage(nullptr);
        mLoaded = true;
        return true;
    }

    void TextureRuntime::pushNextTask()
    {
        auto taskQueue = mProjectRuntimeHandle->getTaskQueue();
        auto gfxTaskQueue = mProjectRuntimeHandle->getGraphicsComponent()->getTaskQueue();

        if (mLoadFromDefinitionTask->hasState(TASK_STATE_QUEUED))
        {
            taskQueue->pushTask(mLoadFromDefinitionTask);
        }
        else if (mLoadFromDefinitionTask->hasState(TASK_STATE_COMPLETED) &&
                 mTextureLoadIntoGLTask->hasState(TASK_STATE_QUEUED))
        {
            gfxTaskQueue->pushTask(mTextureLoadIntoGLTask);
        }
        // For sprite instances
        else
        {
            for(auto entity : mInstances)
            {
				if (entity->getSceneRuntime()->hasState(SCENE_STATE_ACTIVE))
				{

					// Do entity specific tasks
				}
            }
        }
    }
}
