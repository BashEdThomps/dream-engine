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
#include "../../Components/Animation/AnimationInstance.h"
#include "../../Components/Audio/AudioInstance.h"
#include "../../Components/Audio/AudioComponent.h"
#include "../../Components/Graphics/Font/FontInstance.h"
#include "../../Components/Graphics/Model/AssimpModelInstance.h"
#include "../../Components/Graphics/Shader/ShaderInstance.h"
#include "../../Components/Graphics/Light/LightInstance.h"
#include "../../Components/Graphics/Sprite/SpriteInstance.h"
#include "../../Components/Physics/PhysicsObjectInstance.h"
#include "../../Components/Physics/PhysicsComponent.h"

#include "../../Lua/LuaScriptInstance.h"
#include "../../Lua/LuaEngine.h"

#include "../../Components/IAssetDefinition.h"

#include "../../Components/Animation/AnimationDefinition.h"
#include "../../Components/Audio/AudioDefinition.h"
#include "../../Components/Graphics/Font/FontDefinition.h"
#include "../../Components/Graphics/Light/LightDefinition.h"
#include "../../Components/Graphics/Model/ModelDefinition.h"
#include "../../Components/Physics/PhysicsObjectDefinition.h"
#include "../../Components/Graphics/Shader/ShaderDefinition.h"
#include "../../Components/Graphics/Sprite/SpriteDefinition.h"
#include "../../Lua/ScriptDefinition.h"

#include "../../Project/Project.h"
#include "../../Project/ProjectRuntime.h"
#include "../../Project/ProjectDefinition.h"


using std::vector;
using std::flush;

using glm::vec3;

namespace Dream
{
    SceneObjectRuntime::SceneObjectRuntime(SceneObjectDefinition* sdHandle, SceneRuntime* srHandle)
        : // Init list
          Runtime(sdHandle),
          mSceneRuntimeHandle(srHandle),
          mParentRuntimeHandle(nullptr),
          mLoaded(false),
          mHasFocus(false),
          mFollowsCamera(false)

    {
        if (Constants::DEBUG)
        {
            cout << "SceneObjectRuntime: Constructing Object" << endl;
        }

        useDefinition(sdHandle);
    }

    SceneObjectRuntime::~SceneObjectRuntime
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObjectRuntime: Destroying Object" << endl;
        }

        if (hasPhysicsObjectInstance())
        {
            mSceneRuntimeHandle->getProjectRuntimeHandle()
                ->getPhysicsComponentHandle()->removePhysicsObjectInstance(getPhysicsObjectInstance());
        }

        if (hasScriptInstance())
        {
            mSceneRuntimeHandle->getProjectRuntimeHandle()->getLuaEngineHandle()->removeFromScriptMap(this);
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
        return mTransform.getRotation();
    }

    vec3
    SceneObjectRuntime::getScale
    ()
    {
        return mTransform.getScale();
    }

    vec3
    SceneObjectRuntime::getTranslation
    ()
    {
        return mTransform.getTranslation();
    }

    /*
    void
    SceneObjectRuntime::setAnimationInstance
    (AnimationInstance* animationAsset)
    {
        mAnimationInstance.reset(animationAsset);
    }
    */

    AnimationInstance*
    SceneObjectRuntime::getAnimationInstance
    ()
    {
        return mAnimationInstance.get();
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
        if (Constants::DEBUG)
        {
            cout << "SceneObjectRuntime: Collecting Garbage " << getNameAndUuidString() << endl;
        }
        clearEventQueue();
    }

    bool
    SceneObjectRuntime::hasPhysicsObjectInstance
    ()
    {
        return mPhysicsObjectInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasAnimationInstance
    ()
    {
        return mAnimationInstance != nullptr;
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
        mProjectPath = mSceneRuntimeHandle->getProjectRuntimeHandle()->getProjectHandle()->getProjectPath();

        if (Constants::DEBUG)
        {
            cout << "SceneObjectRuntime: Creating Asset Intance of: ("
                 << definition->getType() << ") " << definition->getName() << flush;

            if (mParentRuntimeHandle)
            {
                 cout << ", for  " << mParentRuntimeHandle->getNameAndUuidString() << flush;
            }

            cout << endl;
        }

        if(definition->isTypeAnimation())
        {
            createAnimationInstance(dynamic_cast<AnimationDefinition*>(definition));
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
            cerr << "SceneObjectRuntime: Invalid Asset Instance Type"
                 << definition->getType() << endl;
        }

        setLoadedFlag(true);
    }

    void
    SceneObjectRuntime::createPhysicsObjectInstance
    (PhysicsObjectDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObjectRuntime: Creating Physics Object Asset Instance." << endl;
        }
        mPhysicsObjectInstance.reset
        (
            new PhysicsObjectInstance(definition, this)
        );
        mPhysicsObjectInstance->load(mProjectPath);
    }

    void
    SceneObjectRuntime::createAnimationInstance
    (AnimationDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObjectRuntime: Creating Animation asset instance." << endl;
        }
        mAnimationInstance.reset
        (
            new AnimationInstance(definition,this)
        );
        mAnimationInstance->load(mProjectPath);
    }

    void
    SceneObjectRuntime::createAudioInstance
    (AudioDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObjectRuntime: Creating Audio asset instance." << endl;
        }
        // hottest trainwreck 2017!
        mAudioInstance.reset
        (
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
        if (Constants::DEBUG)
        {
            cout << "SceneObjectRuntime: Creating Model asset instance." << endl;
        }
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
        mModelInstance->load(mProjectPath);
    }

    void
    SceneObjectRuntime::createScriptInstance
    (ScriptDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObjectRuntime: Creating Script asset instance." << endl;
        }
        mScriptInstance.reset(new LuaScriptInstance(definition,this));
        mScriptInstance->load(mProjectPath);
        mSceneRuntimeHandle->getProjectRuntimeHandle()->getLuaEngineHandle()->addToScriptMap(this,mScriptInstance.get());
    }

    void
    SceneObjectRuntime::createShaderInstance
    (ShaderDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObjectRuntime: Creating Shader asset instance." << endl;
        }
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
        if (Constants::DEBUG)
        {
            cout << "SceneObjectRuntime: Creating Light Asset instance." << endl;
        }
        mLightInstance.reset(new LightInstance(definition, this));
        mLightInstance->load(mProjectPath);
    }

    void
    SceneObjectRuntime::createSpriteInstance
    (SpriteDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObjectRuntime: Creating Sprite Asset instance." << endl;
        }
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
        if (Constants::DEBUG)
        {
            cout << "SceneObjectRuntime: Creating Font Asset instance." << endl;
        }
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
        bool retval = funk(this);

        for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
        {
            if ((*it))
            {
                retval = retval || funk((*it).get());
            }
        }
        return retval;
    }

    void*
    SceneObjectRuntime::applyToAll
    (function<void*(SceneObjectRuntime*)> funk)
    {
        void* retval = funk(this);
        if (retval)
        {
            return retval;
        }

        for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
        {
            if ((*it))
            {
                retval = funk((*it).get());
                if (retval)
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
            if (*it)
            {
                if ((*it).get()->hasUuid(uuid))

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

    void
    SceneObjectRuntime::useDefinition
    (IDefinition* iDefinitionHandle)
    {
       SceneObjectDefinition *defHandle = dynamic_cast<SceneObjectDefinition*>(iDefinitionHandle);

       if (Constants::DEBUG)
       {
           cout << "SceneObjectRuntime: Using Definition "
                << defHandle->getNameAndUuidString()
                << endl;
       }

       setName(defHandle->getName());
       setUuid(defHandle->getUuid());
       setTransform(defHandle->getTransform());
       setFollowsCamera(defHandle->followsCamera());
       setAssetDefinitionLoadQueue(defHandle->getAssetDefinitionLoadQueue());
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

}
