/*
 * SceneObjectRuntime.cpp
 *
 * Created: 16 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
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

#include "SceneObjectRuntime.h"

#include <iostream>

#include "SceneObjectDefinition.h"

#include "../SceneRuntime.h"

#include "../../Common/Constants.h"

#include "../../Components/Event.h"
#include "../../Components/Path/PathInstance.h"
#include "../../Components/Audio/AudioInstance.h"
#include "../../Components/Audio/AudioComponent.h"
#include "../../Components/Graphics/Font/FontInstance.h"
#include "../../Components/Graphics/Model/AssimpModelInstance.h"
#include "../../Components/Graphics/Shader/ShaderInstance.h"
#include "../../Components/Graphics/Light/LightInstance.h"
#include "../../Components/Graphics/Sprite/SpriteInstance.h"
#include "../../Components/Physics/PhysicsObjectInstance.h"
#include "../../Components/Physics/PhysicsComponent.h"
#include "../../Components/IAssetDefinition.h"

#include "../../Components/Path/PathDefinition.h"
#include "../../Components/Audio/AudioDefinition.h"
#include "../../Components/Graphics/Font/FontDefinition.h"
#include "../../Components/Graphics/Light/LightDefinition.h"
#include "../../Components/Graphics/Model/ModelDefinition.h"
#include "../../Components/Physics/PhysicsObjectDefinition.h"
#include "../../Components/Graphics/Shader/ShaderDefinition.h"
#include "../../Components/Graphics/Sprite/SpriteDefinition.h"

#include "../../Components/Lua/ScriptDefinition.h"
#include "../../Components/Lua/LuaScriptInstance.h"
#include "../../Components/Lua/LuaComponent.h"



#include "../../Project/Project.h"
#include "../../Project/ProjectRuntime.h"
#include "../../Project/ProjectDefinition.h"

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

using std::vector;
using std::flush;

using glm::vec3;

namespace Dream
{
    SceneObjectRuntime::SceneObjectRuntime(SceneObjectDefinition* sdHandle, SceneRuntime* srHandle)
        : // Init list
          Runtime(sdHandle),
          ILoggable ("SceneObjectRuntime"),
          mSceneRuntimeHandle(srHandle),
          mParentRuntimeHandle(nullptr),
          mLoaded(false),
          mHasFocus(false),
          mFollowsCamera(false)

    {
        auto log = getLog();
        log->trace( "Constructing Object" );
        useDefinition(sdHandle);
    }

    SceneObjectRuntime::~SceneObjectRuntime
    ()
    {
        auto log = getLog();
        log->trace( "Destroying Object" );

        if (hasPhysicsObjectInstance())
        {
            mSceneRuntimeHandle->getProjectRuntimeHandle()
                    ->getPhysicsComponentHandle()->removePhysicsObjectInstance(getPhysicsObjectInstance());
        }

        if (hasScriptInstance())
        {
            mSceneRuntimeHandle->getProjectRuntimeHandle()->getLuaComponentHandle()->removeFromScriptMap(this);
        }
    }

    void
    SceneObjectRuntime::resetTransform
    ()
    {
        resetTranslation();
        resetRotation();
        resetScale();
    }

    void
    SceneObjectRuntime::resetTranslation
    ()
    {
        setTranslation(0.0f, 0.0f, 0.0f);
    }

    void
    SceneObjectRuntime::resetRotation
    ()
    {
        setRotation(0.0f, 0.0f, 0.0f);
    }

    void
    SceneObjectRuntime::resetScale
    ()
    {
        setScale(1.0f, 1.0f, 1.0f);
    }

    void
    SceneObjectRuntime::setTranslation
    (vec3 translation)
    {
        mTransform.setTranslation(translation);
    }

    void
    SceneObjectRuntime::setRotation
    (vec3 rotation)
    {
        mTransform.setRotation(rotation);
    }

    void
    SceneObjectRuntime::setScale
    (vec3 scale)
    {
        mTransform.setScale(scale);
    }

    void
    SceneObjectRuntime::setTranslation
    (float x, float y, float z)
    {
        mTransform.setTranslation(x,y,z);
    }

    void
    SceneObjectRuntime::setRotation
    (float x, float y, float z)
    {
        mTransform.setRotation(x,y,z);
    }

    void
    SceneObjectRuntime::setScale
    (float x, float y, float z)
    {
        mTransform.setScale(x,y,z);
    }

    vec3
    SceneObjectRuntime::getRotation
    ()
    {
        return getTransform().getRotation();
    }

    vec3
    SceneObjectRuntime::getScale
    ()
    {
        return getTransform().getScale();
    }

    vec3
    SceneObjectRuntime::getTranslation
    ()
    {
        return getTransform().getTranslation();
    }

    /*
    void
    SceneObjectRuntime::setPathInstance
    (PathInstance* pathAsset)
    {
        mPathInstance.reset(pathAsset);
    }
    */

    PathInstance*
    SceneObjectRuntime::getPathInstance
    ()
    {
        return mPathInstance.get();
    }

    /*
    void
    SceneObjectRuntime::setAudioInstance
    (AudioInstance* audioAsset)
    {
        mAudioInstance.reset(audioAsset);
    }
    */

    AudioInstance*
    SceneObjectRuntime::getAudioInstance
    ()
    {
        return mAudioInstance.get();
    }

    /*
    void
    SceneObjectRuntime::setModelInstance
    (AssimpModelInstance* modelAsset)
    {
        mModelInstance.reset(modelAsset);
    }
    */

    AssimpModelInstance*
    SceneObjectRuntime::getModelInstance
    ()
    {
        return mModelInstance.get();
    }

    /*
    void
    SceneObjectRuntime::setScriptInstance
    (LuaScriptInstance* scriptAsset)
    {
        mScriptInstance.reset(scriptAsset);
    }
    */

    LuaScriptInstance*
    SceneObjectRuntime::getScriptInstance
    ()
    {
        return mScriptInstance.get();
    }

    /*
    void
    SceneObjectRuntime::setShaderInstance
    (ShaderInstance* shaderAsset)
    {
        mShaderInstance.reset(shaderAsset);
    }
    */

    ShaderInstance*
    SceneObjectRuntime::getShaderInstance
    ()
    {
        return mShaderInstance.get();
    }

    /*
    void
    SceneObjectRuntime::setLightInstance
    (LightInstance* lightAsset)
    {
        mLightInstance.reset(lightAsset);
    }
    */

    LightInstance*
    SceneObjectRuntime::getLightInstance
    ()
    {
        return mLightInstance.get();
    }

    bool
    SceneObjectRuntime::hasLightInstance
    ()
    {
        return mLightInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasModelInstance
    ()
    {
        return mModelInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasShaderInstance
    ()
    {
        return mShaderInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasScriptInstance
    ()
    {
        return mScriptInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasSpriteInstance
    ()
    {
        return mSpriteInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasFontInstance
    ()
    {
        return mFontInstance != nullptr;
    }

    void
    SceneObjectRuntime::addAssetDefinitionUuidToLoad
    (string def)
    {
        mAssetDefinitionUuidLoadQueue.push_back(def);
    }

    vector<string>
    SceneObjectRuntime::getAssetDefinitionUuidsToLoad
    ()
    {
        return mAssetDefinitionUuidLoadQueue;
    }

    /*
    void
    SceneObjectRuntime::setFontInstance
    (FontInstance* font)
    {
        mFontInstance.reset(font);
    }
    */

    FontInstance*
    SceneObjectRuntime::getFontInstance
    ()
    {
        return mFontInstance.get();
    }

    /*
    void
    SceneObjectRuntime::setPhysicsObjectInstance
    (PhysicsObjectInstance* poi)
    {
        return mPhysicsObjectInstance.reset(poi);
    }
    */

    PhysicsObjectInstance*
    SceneObjectRuntime::getPhysicsObjectInstance
    ()
    {
        return mPhysicsObjectInstance.get();
    }

    string
    SceneObjectRuntime::getTransformType
    ()
    {
        return mTransform.getTransformType();
    }

    void
    SceneObjectRuntime::setTransformType
    (string transformType)
    {
        mTransform.setTransformType(transformType);
    }

    Transform3D&
    SceneObjectRuntime::getTransform
    ()
    {
        auto log = getLog();
        if (mTransform.getTransformType().compare(Constants::TRANSFORM_TYPE_OFFSET) == 0)
        {
            log->info("Returning Offset Transform for {}",getNameAndUuidString());
            mOffsetTransform = mTransform.offsetFrom(getParentRuntimeHandle()->getTransform());
            return mOffsetTransform;
        }
        log->info("Returning Absolute Transform for {}", getNameAndUuidString());
        return mTransform;
    }

    void
    SceneObjectRuntime::setTransform
    (Transform3D transform)
    {
        mTransform = transform;
    }

    bool
    SceneObjectRuntime::hasFocus
    ()
    {
        return mHasFocus;
    }

    void
    SceneObjectRuntime::setHasFocus
    (bool focus)
    {
        mHasFocus = focus;
    }

    SpriteInstance*
    SceneObjectRuntime::getSpriteInstance
    ()
    {
        return mSpriteInstance.get();
    }

    bool
    SceneObjectRuntime::getLoadedFlag
    ()
    {
        return mLoaded;
    }

    void
    SceneObjectRuntime::setLoadedFlag
    (bool loaded)
    {
        mLoaded = loaded;
    }

    bool
    SceneObjectRuntime::hasEvents
    ()
    {
        return !mEventQueue.empty();
    }

    void
    SceneObjectRuntime::sendEvent
    (Event event)
    {
        mEventQueue.push_back(event);
    }

    vector<Event>
    SceneObjectRuntime::getEventQueue
    ()
    {
        return mEventQueue;
    }

    void SceneObjectRuntime::clearEventQueue()
    {
        mEventQueue.clear();
    }

    void SceneObjectRuntime::collectGarbage()
    {
        auto log = getLog();
        log->info( "Collecting Garbage {}" ,getNameAndUuidString() );
        clearEventQueue();
    }

    bool
    SceneObjectRuntime::hasPhysicsObjectInstance
    ()
    {
        return mPhysicsObjectInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasPathInstance
    ()
    {
        return mPathInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasAudioInstance
    ()
    {
        return mAudioInstance != nullptr;
    }

    void
    SceneObjectRuntime::createAssetInstances
    ()
    {
        for (string aDefUuid : mAssetDefinitionUuidLoadQueue)
        {
            createAssetInstanceFromAssetDefinitionByUuid(aDefUuid);
        }
        mAssetDefinitionUuidLoadQueue.clear();
    }

    void
    SceneObjectRuntime::createAssetInstanceFromAssetDefinitionByUuid
    (string uuid)
    {
        IAssetDefinition* assetDefinition;
        assetDefinition = mSceneRuntimeHandle->getProjectRuntimeHandle()
                ->getProjectHandle()->getProjectDefinitionHandle()
                ->getAssetDefinitionHandleByUuid(uuid);
        createAssetInstance(assetDefinition);
    }

    void
    SceneObjectRuntime::createAssetInstance
    (IAssetDefinition* definition)
    {
        auto log = getLog();
        mProjectPath = mSceneRuntimeHandle->getProjectRuntimeHandle()->getProjectHandle()->getProjectPath();

        log->info( "Creating Asset Intance of: ({}) {}", definition->getType() ,  definition->getName());

        if (mParentRuntimeHandle)
        {
            log->info( " for {} " ,mParentRuntimeHandle->getNameAndUuidString());
        }

        if(definition->isTypePath())
        {
            createPathInstance(dynamic_cast<PathDefinition*>(definition));
        }
        else if (definition->isTypeAudio())
        {
            createAudioInstance(dynamic_cast<AudioDefinition*>(definition));
        }
        else if (definition->isTypeModel())
        {
            createModelInstance(dynamic_cast<ModelDefinition*>(definition));
        }
        else if (definition->isTypeScript())
        {
            createScriptInstance(dynamic_cast<ScriptDefinition*>(definition));
        }
        else if (definition->isTypeShader())
        {
            createShaderInstance(dynamic_cast<ShaderDefinition*>(definition));
        }
        else if (definition->isTypePhysicsObject())
        {
            createPhysicsObjectInstance(dynamic_cast<PhysicsObjectDefinition*>(definition));
        }
        else if (definition->isTypeLight())
        {
            createLightInstance(dynamic_cast<LightDefinition*>(definition));
        }
        else if (definition->isTypeSprite())
        {
            createSpriteInstance(dynamic_cast<SpriteDefinition*>(definition));
        }
        else if (definition->isTypeFont())
        {
            createFontInstance(dynamic_cast<FontDefinition*>(definition));
        }
        else
        {
            log->error( "Invalid Asset Instance Type {}",definition->getType() );
        }

        setLoadedFlag(true);
    }

    void
    SceneObjectRuntime::createPhysicsObjectInstance
    (PhysicsObjectDefinition* definition)
    {
        auto log = getLog();
        log->info( "Creating Physics Object Asset Instance." );
        mPhysicsObjectInstance.reset ( new PhysicsObjectInstance(definition, this));
        mPhysicsObjectInstance->load(mProjectPath);
    }

    void
    SceneObjectRuntime::createPathInstance
    (PathDefinition* definition)
    {
        auto log = getLog();
        log->info( "Creating Path asset instance." );
        mPathInstance.reset(new PathInstance(definition,this));
        mPathInstance->load(mProjectPath);
    }

    void
    SceneObjectRuntime::createAudioInstance
    (AudioDefinition* definition)
    {
        auto log = getLog();
        log->info( "Creating Audio asset instance." );
        // hottest trainwreck 2017!
        mAudioInstance.reset(
            mSceneRuntimeHandle
                ->getProjectRuntimeHandle()
                    ->getAudioComponentHandle()
                        ->newAudioInstance(definition,this)
        );
        mAudioInstance->load(mProjectPath);
    }

    void
    SceneObjectRuntime::createModelInstance
    (ModelDefinition* definition)
    {
        auto log = getLog();
        log->info( "Creating Model asset instance." );
        mModelInstance.reset
                (
                    new AssimpModelInstance
                    (
                        mSceneRuntimeHandle->getProjectRuntimeHandle()->getModelCacheHandle(),
                        mSceneRuntimeHandle->getProjectRuntimeHandle()->getTextureCacheHandle(),
                        definition,
                        this
                        )
                    );
        try {
            mModelInstance->load(mProjectPath);
        }
        catch (std::exception e)
        {
            log->error("Exception while loading model resource {}",e.what());
            return;
        }
    }

    void
    SceneObjectRuntime::createScriptInstance
    (ScriptDefinition* definition)
    {
        auto log = getLog();
        log->info( "Creating Script asset instance." );
        mScriptInstance.reset(new LuaScriptInstance(definition,this));
        mScriptInstance->load(mProjectPath);
        mSceneRuntimeHandle->getProjectRuntimeHandle()->getLuaComponentHandle()->addToScriptMap(this,mScriptInstance.get());
    }

    void
    SceneObjectRuntime::createShaderInstance
    (ShaderDefinition* definition)
    {
        auto log = getLog();
        log->info( "Creating Shader asset instance." );
        mShaderInstance.reset
                (
                    new ShaderInstance
                    (
                        mSceneRuntimeHandle->getProjectRuntimeHandle()->getShaderCacheHandle(),
                        definition,
                        this
                        )
                    );
        mShaderInstance->load(mProjectPath);
    }

    void
    SceneObjectRuntime::createLightInstance
    (LightDefinition* definition)
    {
        auto log = getLog();
        log->info( "Creating Light Asset instance." );
        mLightInstance.reset(new LightInstance(definition, this));
        mLightInstance->load(mProjectPath);
    }

    void
    SceneObjectRuntime::createSpriteInstance
    (SpriteDefinition* definition)
    {
        auto log = getLog();
        log->info( "Creating Sprite Asset instance." );
        mSpriteInstance.reset
                (
                    new SpriteInstance
                    (
                        mSceneRuntimeHandle->getProjectRuntimeHandle()->getTextureCacheHandle(),
                        definition,
                        this
                        )
                    );
        mSpriteInstance->load(mProjectPath);
    }

    void
    SceneObjectRuntime::createFontInstance
    (FontDefinition* definition)
    {
        auto log = getLog();
        log->info( "Creating Font Asset instance." );
        mFontInstance.reset
                (
                    new FontInstance
                    (
                        mSceneRuntimeHandle->getProjectRuntimeHandle()->getFontCacheHandle(),
                        definition,
                        this
                        )
                    );
        mFontInstance->load(mProjectPath);
    }

    bool
    SceneObjectRuntime::applyToAll
    (function<bool(SceneObjectRuntime*)> funk)
    {
        auto log = getLog();
        log->trace(
            "{}::applyToAll(bool) applying to {} children",
            getNameAndUuidString(),
            mChildRuntimes.size()
        );

        bool retval = funk(this);

        for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
        {
            if ((*it))
            {
                retval = retval || (*it)->applyToAll(funk);
            }
        }
        return retval;
    }

    void*
    SceneObjectRuntime::applyToAll
    (function<void*(SceneObjectRuntime*)> funk)
    {
        auto log = getLog();
        log->trace(
            "{}::applyToAll(void*) applying to {} children",
            getNameAndUuidString(),
            mChildRuntimes.size()
        );

        void* retval = funk(this);
        if (retval != nullptr)
        {
            return retval;
        }

        for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
        {
            if ((*it))
            {
                retval = (*it)->applyToAll(funk);
                if (retval != nullptr)
                {
                    return retval;
                }
            }
        }
        return nullptr;
    }

    SceneObjectRuntime*
    SceneObjectRuntime::getChildRuntimeHandleByUuid
    (string uuid)
    {
        for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
        {
            if (*it != nullptr)
            {
                if ((*it)->hasUuid(uuid))

                {
                    return (*it).get();
                }
            }
        }
        return nullptr;
    }

    bool
    SceneObjectRuntime::isParentOf
    (SceneObjectRuntime *child)
    {
        for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
        {
            if ((*it).get() == child)
            {
                return true;
            }
        }
        return false;
    }

    void
    SceneObjectRuntime::setParentRuntimeHandle
    (SceneObjectRuntime *parent)
    {
        mParentRuntimeHandle = parent;
    }

    SceneObjectRuntime*
    SceneObjectRuntime::getParentRuntimeHandle
    ()
    {
        return mParentRuntimeHandle;
    }

    SceneRuntime*
    SceneObjectRuntime::getSceneRuntimeHandle
    ()
    {
        return mSceneRuntimeHandle;
    }

    SceneObjectDefinition*SceneObjectRuntime::getSceneObjectDefinitionHandle()
    {
       return dynamic_cast<SceneObjectDefinition*>(getDefinitionHandle());
    }

    void
    SceneObjectRuntime::useDefinition
    (IDefinition* iDefinitionHandle)
    {
        SceneObjectDefinition *defHandle = dynamic_cast<SceneObjectDefinition*>(iDefinitionHandle);
        auto log = getLog();
        log->info( "Using Definition {}", defHandle->getNameAndUuidString());
        setName(defHandle->getName());
        setUuid(defHandle->getUuid());
        setTransform(defHandle->getTransform());
        setFollowsCamera(defHandle->followsCamera());
        setAssetDefinitionLoadQueue(defHandle->getAssetDefinitionLoadQueue());
        setHasFocus(defHandle->hasFocus());
        createAssetInstances();
        loadChildrenFromDefinition(defHandle);
    }

    void
    SceneObjectRuntime::setAssetDefinitionLoadQueue
    (vector<string> loadQueue)
    {
        mAssetDefinitionUuidLoadQueue = loadQueue;
    }

    void
    SceneObjectRuntime::loadChildrenFromDefinition
    (SceneObjectDefinition* definitionHandle)
    {
        vector<SceneObjectDefinition*> definitions = definitionHandle->getChildDefinitionsHandleList();
        for
                (auto it = begin(definitions); it != end(definitions); it++)
        {
            SceneObjectRuntime* child = new SceneObjectRuntime(*it, mSceneRuntimeHandle);
            child->setParentRuntimeHandle(this);
            mChildRuntimes.push_back(unique_ptr<SceneObjectRuntime>(child));
        }
    }

    bool
    SceneObjectRuntime::followsCamera
    () const
    {
        return mFollowsCamera;
    }

    void
    SceneObjectRuntime::setFollowsCamera
    (bool followsCamera)
    {
        mFollowsCamera = followsCamera;
    }

    void
    SceneObjectRuntime::walk
    (float leftStickX, float leftStickY)
    {
        auto log = getLog();
        log->trace("Walk: ({},{}) ",leftStickX,leftStickY);

        if (leftStickX == 0.0f && leftStickY == 0.0f)
        {
            return;
        }

        // Translate to current
        mat4 mtx;
        mtx = glm::translate(mtx,mTransform.getTranslation());
         // Rotate to current
        mat4 rotMat = mat4_cast(mTransform.getOrientation());
        mtx = mtx * rotMat;

        // Rotate to new
        quat newRotation = glm::rotate(leftStickX,vec3(0,1,0));
        mat4 newRotMat = mat4_cast(newRotation);
        mtx = mtx * newRotMat;

        // Translate to new
        mtx = glm::translate(mtx,vec3(0,0,-leftStickY));
        mTransform.setOrientation(quat(mtx));
        mTransform.setTranslation(glm::vec3(mtx[3]));

        if (mPhysicsObjectInstance != nullptr)
        {
            auto rb = mPhysicsObjectInstance->getRigidBody();
            if (rb!=nullptr)
            {
                rb->setCenterOfMassTransform(mTransform.getTransformAsBtTransform());
            }
        }
    }
}
