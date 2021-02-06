#include "SpriteRuntime.h"

#include "SpriteDefinition.h"

#include "Common/Logger.h"
#include "Components/Graphics/Texture/TextureCache.h"
#include "Components/Graphics/Texture/TextureRuntime.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Scene/SceneRuntime.h"
#include "Scene/Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"
#include "Project/Project.h"

using std::make_shared;
using std::max;


namespace octronic::dream
{
    SpriteRuntime::SpriteRuntime(SpriteDefinition* fd, EntityRuntime* er)
        : DiscreteAssetRuntime("SpriteRuntime",fd, er),
          mTexture(nullptr),
          mWidth(0),
          mHeight(0),
          mSpriteConstructionTask(this),
          mSpriteDestructionTask(nullptr)
    {
        LOG_TRACE("SpriteRuntime: {}", __FUNCTION__);
    }

    SpriteRuntime::~SpriteRuntime()
    {
        LOG_TRACE("SpriteRuntime: {}", __FUNCTION__);
        if (mTexture)
        {
			mSpriteDestructionTask = make_shared<SpriteDestructionTask>(mTexture, mEntityRuntime->getUuid());
			ProjectRuntime* projectRuntime = mEntityRuntime->getSceneRuntime()->getProjectRuntime();
			projectRuntime->getGraphicsComponent()->pushDestructionTask(mSpriteDestructionTask);
        }
    }

    void SpriteRuntime::pushConstructionTask()
    {
        if(dreamTryLock()) {
            dreamLock();
            LOG_TRACE("SpriteRuntime: {}", __FUNCTION__);
            ProjectRuntime* projectRuntime = mEntityRuntime->getSceneRuntime()->getProjectRuntime();
            projectRuntime->getGraphicsComponent()->pushTask(&mSpriteConstructionTask);
        } dreamElseLockFailed
    }

    bool SpriteRuntime::useDefinition()
    {
        if(dreamTryLock())
        {
            dreamLock();

            LOG_TRACE("SpriteRuntime: {}", __FUNCTION__);
            SpriteDefinition* spriteDef = static_cast<SpriteDefinition*>(mDefinition);
            ProjectRuntime* projectRuntime = mEntityRuntime->getSceneRuntime()->getProjectRuntime();
            TextureCache* textureCache = projectRuntime->getTextureCache();

            mWidth = spriteDef->getWidth();
            mHeight = spriteDef->getHeight();
            UuidType textureUuid = spriteDef->getTexture();
            mTexture = static_cast<TextureRuntime*>(textureCache->getRuntime(textureUuid));

            // Push back construction task
            pushConstructionTask();

            return true;
        } dreamElseLockFailed
    }

    int SpriteRuntime::getWidth() const
    {
        return mWidth;
    }

    int SpriteRuntime::getHeight() const
    {
        return mHeight;
    }

    void SpriteRuntime::setWidth(int size)
    {
        if(dreamTryLock()) {
            dreamLock();

            mWidth = size;
        } dreamElseLockFailed
    }

    void SpriteRuntime::setHeight(int size)
    {
        if(dreamTryLock()) {
            dreamLock();

            mHeight = size;
        } dreamElseLockFailed
    }

    void SpriteRuntime::draw()
    {
            LOG_TRACE("SpriteRuntime: {}", __FUNCTION__);

            if (mSpriteConstructionTask.getState() != TaskState::TASK_STATE_COMPLETED)
            {
                LOG_ERROR("SpriteRuntime: tried to draw before construction task was completed");
                assert(false);
            }

            // Do drawing here
    }

    TextureRuntime* SpriteRuntime::getTextureRuntime() const
    {
        return mTexture;
    }

    void SpriteRuntime::pushInstance()
    {
        if (mTexture)
        {
            mTexture->pushSpriteInstance(this);
        }
    }

    void SpriteRuntime::popInstance()
    {
        if (mTexture)
        {
            mTexture->popSpriteInstance(this);
        }
    }
}
