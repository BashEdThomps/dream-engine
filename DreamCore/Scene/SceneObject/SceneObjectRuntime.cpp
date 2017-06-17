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

#include "SceneObject.h"
#include "SceneObjectDefinition.h"

#include "../Scene.h"

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

#include "../../Lua/LuaScriptInstance.h"
#include "../../Lua/LuaEngine.h"

#include "../../Project/AssetDefinition.h"
#include "../../Project/Project.h"
#include "../../Project/ProjectRuntime.h"


using std::vector;
using glm::vec3;

namespace Dream
{
    SceneObjectRuntime::SceneObjectRuntime(SceneObject* owner)
        : // Init list
          Runtime(),
          mOwnerHandle(owner),
          mLoaded(false),
          mHasFocus(false),
          mDelete(false)

    {
        if (Constants::DEBUG)
        {
            cout << "SceneObjectRuntime: Constructing Object" << endl;
        }
    }

    SceneObjectRuntime::~SceneObjectRuntime()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObjectRuntime: Destroying Object" << endl;
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
    SceneObjectRuntime::deleteChildRuntimes
    ()
    {
        // TODO - Fix Me
        /*
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Deleting " << mChildren.size()
                 << "children of "
                 << mObjectHandle->getNameAndUuidString() << endl;
        }

        for (SceneObject* child : mChildren)
        {
            child->resetRuntime();
        }
        */
    }

    void
    SceneObjectRuntime::deleteAssetInstances
    ()
    {
        if (Constants::DEBUG)
        {
            /* TODO - Fix Me
            cout << "SceneObject: Deleting asset instances for "
                 << mSceneHandle->getNameAndUuidString() << endl;
            */
        }

        mAudioInstance.reset();
        mAnimationInstance.reset();
        mModelInstance.reset();
        mShaderInstance.reset();
        mLightInstance.reset();
        mSpriteInstance.reset();
        mScriptInstance.reset();
        mPhysicsObjectInstance.reset();
        mFontInstance.reset();
    }

    void
    SceneObjectRuntime::setTranslation
    (vec3 translation)
    {
        mTransform->setTranslation(translation);
    }

    void
    SceneObjectRuntime::setRotation
    (vec3 rotation)
    {
        mTransform->setRotation(rotation);
    }

    void
    SceneObjectRuntime::setScale
    (vec3 scale)
    {
        mTransform->setScale(scale);
    }

    void
    SceneObjectRuntime::setTranslation
    (float x, float y, float z)
    {
        mTransform->setTranslation(x,y,z);
    }

    void
    SceneObjectRuntime::setRotation
    (float x, float y, float z)
    {
        mTransform->setRotation(x,y,z);
    }

    void
    SceneObjectRuntime::setScale
    (float x, float y, float z)
    {
        mTransform->setScale(x,y,z);
    }

    vec3
    SceneObjectRuntime::getRotation
    ()
    {
        return mTransform->getRotation();
    }

    vec3
    SceneObjectRuntime::getScale
    ()
    {
        return mTransform->getScale();
    }

    vec3
    SceneObjectRuntime::getTranslation
    ()
    {
        return mTransform->getTranslation();
    }

    void
    SceneObjectRuntime::setAnimationInstance
    (AnimationInstance* animationAsset)
    {
        mAnimationInstance.reset(animationAsset);
    }

    AnimationInstance*
    SceneObjectRuntime::getAnimationInstance
    ()
    {
        return mAnimationInstance.get();
    }

    void
    SceneObjectRuntime::setAudioInstance
    (AudioInstance* audioAsset)
    {
        mAudioInstance.reset(audioAsset);
    }

    AudioInstance*
    SceneObjectRuntime::getAudioInstance
    ()
    {
        return mAudioInstance.get();
    }

    void
    SceneObjectRuntime::setModelInstance
    (AssimpModelInstance* modelAsset)
    {
        mModelInstance.reset(modelAsset);
    }

    AssimpModelInstance*
    SceneObjectRuntime::getModelInstance
    ()
    {
        return mModelInstance.get();
    }

    void
    SceneObjectRuntime::setScriptInstance
    (LuaScriptInstance* scriptAsset)
    {
        mScriptInstance.reset(scriptAsset);
    }

    LuaScriptInstance*
    SceneObjectRuntime::getScriptInstance
    ()
    {
        return mScriptInstance.get();
    }

    void
    SceneObjectRuntime::setShaderInstance
    (ShaderInstance* shaderAsset)
    {
        mShaderInstance.reset(shaderAsset);
    }

    ShaderInstance*
    SceneObjectRuntime::getShaderInstance
    ()
    {
        return mShaderInstance.get();
    }

    void
    SceneObjectRuntime::setLightInstance
    (LightInstance* lightAsset)
    {
        mLightInstance.reset(lightAsset);
    }

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
    SceneObjectRuntime::setFontInstance
    (FontInstance* font)
    {
        mFontInstance.reset(font);
    }

    FontInstance*
    SceneObjectRuntime::getFontInstance
    ()
    {
        return mFontInstance.get();
    }

    void
    SceneObjectRuntime::setPhysicsObjectInstance
    (PhysicsObjectInstance* poi)
    {
        return mPhysicsObjectInstance.reset(poi);
    }

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
        return mTransform->getTransformType();
    }

    void
    SceneObjectRuntime::setTransformType
    (string transformType)
    {
        mTransform->setTransformType(transformType);
    }

    Transform3D*
    SceneObjectRuntime::getTransform
    () {
        return mTransform.get();
    }

    void
    SceneObjectRuntime::setTransform
    (Transform3D* transform)
    {
        mTransform.reset(transform);
    }

    void
    SceneObjectRuntime::setHasFocus
    (bool focus)
    {
       mHasFocus = focus;
    }

    void SceneObjectRuntime::setSpriteInstance(SpriteInstance* spriteAsset)
    {
        mSpriteInstance.reset(spriteAsset);
    }

    SpriteInstance*
    SceneObjectRuntime::getSpriteInstance
    ()
    {
        return mSpriteInstance.get();
    }

    void
    SceneObjectRuntime::setDeleteFlag
    (bool del)
    {
        mDelete = del;
    }

    bool
    SceneObjectRuntime::getDeleteFlag
    ()
    {
        return mDelete;
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

    vector<Event>&
    SceneObjectRuntime::getEventQueue
    ()
    {
        return mEventQueue;
    }

    void
    SceneObjectRuntime::cleanUp
    ()
    {
        cleanUpEvents();
        deleteAssetInstances();
        setLoadedFlag(false);
        setDeleteFlag(false);
    }

    void
    SceneObjectRuntime::cleanUpEvents
    ()
    {
        mEventQueue.clear();
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
        for (string aDefUuid : getAssetDefUuidsToLoad())
        {
            createAssetInstanceFromAssetDefinitionByUuid(aDefUuid);
        }
    }

    void
    SceneObjectRuntime::createAssetInstanceFromAssetDefinitionByUuid
    (string uuid)
    {
        AssetDefinition* assetDefinition;
        assetDefinition = mOwnerHandle->getSceneHandle()->getProjectHandle()
                                      ->getAssetDefinitionHandleByUuid(uuid);
        createAssetInstance(assetDefinition);
    }

    void
    SceneObjectRuntime::createAssetInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Asset Intance of: ("
                 << definition->getType() << ") " << definition->getName()
                 << ", for  " << mOwnerHandle->getDefinitionHandle()->getNameAndUuidString()
                 << endl;
        }

        if(definition->isTypeAnimation())
        {
            createAnimationInstance(definition);
        }
        else if (definition->isTypeAudio())
        {
            createAudioInstance(definition);
        }
        else if (definition->isTypeModel())
        {
            createModelInstance(definition);
        }
        else if (definition->isTypeScript())
        {
            createScriptInstance(definition);
        }
        else if (definition->isTypeShader())
        {
            createShaderInstance(definition);
        }
        else if (definition->isTypePhysicsObject())
        {
            createPhysicsObjectInstance(definition);
        }
        else if (definition->isTypeLight())
        {
            createLightInstance(definition);
        }
        else if (definition->isTypeSprite())
        {
            createSpriteInstance(definition);
        }
        else if (definition->isTypeFont())
        {
            createFontInstance(definition);
        }
    }

    void
    SceneObjectRuntime::loadAssetInstances
    ()
    {
        string projectPath = mOwnerHandle->getSceneHandle()->getProjectHandle()
                                         ->getRuntimeHandle()->getProjectPath();

        if (Constants::DEBUG)
        {
            cout << "SceneObject: Loading Asset Data from "
                 << projectPath << endl;
        }

        if(mAudioInstance)
        {
            mAudioInstance->load(projectPath);
        }

        if(mAnimationInstance)
        {
            mAnimationInstance->load(projectPath);
        }

        if(mModelInstance)
        {
            mModelInstance->load(projectPath);
        }

        if(mShaderInstance)
        {
            mShaderInstance->load(projectPath);
        }

        if(mLightInstance)
        {
            mLightInstance->load(projectPath);
        }

        if(mSpriteInstance)
        {
            mSpriteInstance->load(projectPath);
        }

        if(mScriptInstance)
        {
            mScriptInstance->load(projectPath);
        }

        if(mPhysicsObjectInstance)
        {
            mPhysicsObjectInstance->load(projectPath);
        }

        if(mFontInstance)
        {
            mFontInstance->load(projectPath);
        }

        setLoadedFlag(true);
    }

    void
    SceneObjectRuntime::createPhysicsObjectInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Physics Object Asset Instance." << endl;
        }
        mPhysicsObjectInstance.reset
        (
            new PhysicsObjectInstance(definition, mTransform.get())
        );
    }

    void
    SceneObjectRuntime::createAnimationInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Animation asset instance." << endl;
        }
        mAnimationInstance.reset(new AnimationInstance(definition,mTransform.get()));
    }

    void
    SceneObjectRuntime::createAudioInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Audio asset instance." << endl;
        }
        // hottest trainwreck 2017!
        mAudioInstance.reset(
            mOwnerHandle->getSceneHandle()
                 ->getProjectHandle()
                 ->getRuntimeHandle()
                 ->getAudioComponentHandle()
                 ->newAudioInstance(definition,mTransform.get())
       );
    }

    void
    SceneObjectRuntime::createModelInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Model asset instance." << endl;
        }
        mModelInstance.reset
                (
                    new AssimpModelInstance(definition,mTransform.get())
                    );
    }

    void
    SceneObjectRuntime::createScriptInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Script asset instance." << endl;
        }
        mScriptInstance.reset(new LuaScriptInstance(definition, mTransform.get()));
        mOwnerHandle->getSceneHandle()->getProjectHandle()->getRuntimeHandle()
                    ->getLuaEngineHandle()->addToScriptMap(mOwnerHandle,mScriptInstance.get());
    }

    void
    SceneObjectRuntime::createShaderInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Shader asset instance." << endl;
        }
        mShaderInstance.reset(new ShaderInstance(definition,mTransform.get()));
    }

    void
    SceneObjectRuntime::createLightInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Light Asset instance." << endl;
        }
        mLightInstance.reset(new LightInstance(definition,mTransform.get()));
    }

    void
    SceneObjectRuntime::createSpriteInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Sprite Asset instance." << endl;
        }
        mSpriteInstance.reset(new SpriteInstance(definition,mTransform.get()));
    }

    void
    SceneObjectRuntime::createFontInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Font Asset instance." << endl;
        }
        mFontInstance.reset(new FontInstance(definition,mTransform.get()));
    }

}
