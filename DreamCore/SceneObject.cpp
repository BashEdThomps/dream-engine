
#include "SceneObject.h"

#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <algorithm>
#include "String.h"
#include "Uuid.h"
#include "Project.h"
#include "Scene.h"


namespace Dream
{
    SceneObject::SceneObject
    (Scene* scene)
        : mScene(scene),
          mUuid(Uuid::generateUuid())
    {
        constructorInit();
    }

    SceneObject::SceneObject
    (Scene* scene, nlohmann::json soJson)
        : mScene(scene),
          mJson(soJson)
    {
        constructorInit();
        loadJsonData(soJson);
    }

    void
    SceneObject::constructorInit
    ()
    {
        mTransform.reset(new Transform3D());
        mLoaded = false;
        mDelete = false;
        mHasFocus = false;
        mParentHandle = nullptr;
        mAudioInstance = nullptr;
        mAnimationInstance = nullptr;
        mModelInstance  = nullptr;
        mShaderInstance = nullptr;
        mLightInstance = nullptr;
        mSpriteInstance = nullptr;
        mScriptInstance = nullptr;
        mPhysicsObjectInstance = nullptr;
        mFontInstance = nullptr;
    }

    void
    SceneObject::loadJsonData
    (nlohmann::json soJson)
    {
        if(!soJson[Constants::SCENE_OBJECT_UUID].is_null())
        {
            mUuid = soJson[Constants::SCENE_OBJECT_UUID];
        }

        if (!soJson[Constants::SCENE_OBJECT_NAME].is_null())
        {
            mName = soJson[Constants::SCENE_OBJECT_NAME];
        }

        if (!soJson[Constants::SCENE_OBJECT_TRANSFORM_TYPE].is_null())
        {
            string transformType = soJson[Constants::SCENE_OBJECT_TRANSFORM_TYPE];
            mTransform->setTransformType(transformType);
        }
        else
        {
            string transformType = Constants::SCENE_OBJECT_TRANSFORM_TYPE_OFFSET;
            mTransform->setTransformType(transformType);
        }

        if (!soJson[Constants::SCENE_OBJECT_TRANSLATION].is_null())
        {
            nlohmann::json translation = soJson[Constants::SCENE_OBJECT_TRANSLATION];
            setTranslation(translation[Constants::SCENE_OBJECT_X], translation[Constants::SCENE_OBJECT_Y], translation[Constants::SCENE_OBJECT_Z]);
        }
        else
        {
            resetTranslation();
        }

        if (!soJson[Constants::SCENE_OBJECT_ROTATION].is_null())
        {
            nlohmann::json rotation = soJson[Constants::SCENE_OBJECT_ROTATION];
            setRotation(rotation[Constants::SCENE_OBJECT_X], rotation[Constants::SCENE_OBJECT_Y], rotation[Constants::SCENE_OBJECT_Z]);
        }
        else
        {
            resetRotation();
        }

        if (!soJson[Constants::SCENE_OBJECT_SCALE].is_null())
        {
            nlohmann::json scale = soJson[Constants::SCENE_OBJECT_SCALE];
            setScale(scale[Constants::SCENE_OBJECT_X], scale[Constants::SCENE_OBJECT_Y], scale[Constants::SCENE_OBJECT_Z]);
        }
        else
        {
            resetScale();
        }

        if(!soJson[Constants::SCENE_OBJECT_ASSET_INSTANCES].is_null())
        {
            loadAssetDefinitionsToLoadJsonData(soJson[Constants::SCENE_OBJECT_ASSET_INSTANCES]);
        }

        if(!soJson[Constants::SCENE_OBJECT_HAS_FOCUS].is_null())
        {
            bool focus = soJson[Constants::SCENE_OBJECT_HAS_FOCUS];
            setHasFocus(focus);
        }
    }

    void
    SceneObject::loadAssetDefinitionsToLoadJsonData
    (nlohmann::json assetInstancesJson)
    {
        for (nlohmann::json it : assetInstancesJson)
        {
            mAssetDefUuidsToLoad.push_back(it);
        }
    }

    void
    SceneObject::resetTransform
    ()
    {
        resetTranslation();
        resetRotation();
        resetScale();
    }

    void
    SceneObject::resetTranslation
    ()
    {
        setTranslation(0.0f, 0.0f, 0.0f);
    }

    void
    SceneObject::resetRotation
    ()
    {
        setRotation(0.0f, 0.0f, 0.0f);
    }

    void
    SceneObject::resetScale
    ()
    {
        setScale(1.0f, 1.0f, 1.0f);
    }


    SceneObject::~SceneObject
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Destroying Object "
                 << getNameAndUuidString() << endl;
        }
    }

    void
    SceneObject::deleteChildren
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Deleting " << mChildren.size()
                 << "children of "
                 << getNameAndUuidString() << endl;
        }
        for (SceneObject* child : mChildren)
        {
            delete child;
        }
        mChildren.clear();
    }


    void
    SceneObject::deleteAssetInstances
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Deleting asset instances for "
                 << getNameAndUuidString() << endl;
        }

        if (mAudioInstance != nullptr)
        {
            mAudioInstance.reset();
        }

        if (mAnimationInstance != nullptr)
        {
            mAnimationInstance.reset();
        }

        if (mModelInstance != nullptr)
        {
            mModelInstance.reset();
        }

        if (mShaderInstance != nullptr)
        {
            mShaderInstance.reset();
        }

        if (mLightInstance != nullptr)
        {
            mLightInstance.reset();
        }

        if (mSpriteInstance != nullptr)
        {
            mSpriteInstance.reset();
        }

        if (mScriptInstance != nullptr)
        {
            mScriptInstance.reset();
        }

        if (mPhysicsObjectInstance != nullptr)
        {
            mPhysicsObjectInstance.reset();
        }

        if (mFontInstance != nullptr)
        {
            mFontInstance.reset();
        }
    }

    bool
    SceneObject::hasName
    (string name)
    {
        return mName == name;
    }

    void
    SceneObject::setName
    (string name)
    {
        mName = name;
    }

    string
    SceneObject::getName
    ()
    {
        return mName;
    }

    void
    SceneObject::setTranslation
    (glm::vec3 translation)
    {
        mTransform->setTranslation(translation);
    }

    void
    SceneObject::setRotation
    (glm::vec3 rotation)
    {
        mTransform->setRotation(rotation);
    }

    void
    SceneObject::setScale
    (glm::vec3 scale)
    {
        mTransform->setScale(scale);
    }

    void
    SceneObject::setTranslation
    (float x, float y, float z)
    {
        mTransform->setTranslation(x,y,z);
    }

    void
    SceneObject::setRotation
    (float x, float y, float z)
    {
        mTransform->setRotation(x,y,z);
    }

    void
    SceneObject::setScale
    (float x, float y, float z)
    {
        mTransform->setScale(x,y,z);
    }

    glm::vec3
    SceneObject::getRotation
    ()
    {
        return mTransform->getRotation();
    }

    glm::vec3
    SceneObject::getScale
    ()
    {
        return mTransform->getScale();
    }

    glm::vec3
    SceneObject::getTranslation
    ()
    {
        return mTransform->getTranslation();
    }


    bool
    SceneObject::hasUuid
    (string uuid)
    {
        return (mUuid.compare(uuid) == 0);
    }


    void
    SceneObject::setUuid
    (string uuid)
    {
        mUuid = uuid;
    }

    string
    SceneObject::getUuid
    ()
    {
        return mUuid;
    }

    int
    SceneObject::countAllChildren
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Count All Children, Not Implemented" << endl;
        }
        return -1;
    }

    size_t
    SceneObject::countChildren
    ()
    {
        return mChildren.size();
    }

    void
    SceneObject::addChild
    (SceneObject* child)
    {
        child->setParent(this);
        mChildren.push_back(child);
    }

    void
    SceneObject::removeChild
    (SceneObject* child)
    {
        mChildren.erase(remove(mChildren.begin(), mChildren.end(), child), mChildren.end());
    }

    bool
    SceneObject::isChildOf
    (SceneObject* parent)
    {
        return mParentHandle == parent;
    }

    bool
    SceneObject::isParentOf
    (SceneObject* sceneObject)
    {
        return sceneObject->getParent() == this;
    }

    string
    SceneObject::getNameAndUuidString
    ()
    {
        return getUuid()+" : "+getName();
    }

    void
    SceneObject::showStatus
    ()
    {
        cout << "SceneObject:" << endl;
        cout << "          Uuid: " << mUuid << endl;
        cout << "          Name: " << mName << endl;

        if (mParentHandle != nullptr)
        {
            cout << "    ParentUuid: " << mParentHandle->getUuid() << endl;
        }

        cout << "      Children: " << mChildren.size() << endl;
        cout << "Trnasform Type: " << mTransform->getTransformType() << endl;
        cout << "   Translation: " << String::vec3ToString(getTranslation()) << endl;
        cout << "      Rotation: " << String::vec3ToString(getRotation())<< endl;
        cout << "         Scale: " << String::vec3ToString(getScale())<< endl;
    }

    vector<string>
    SceneObject::getAssetDefUuidsToLoad
    ()
    {
        return mAssetDefUuidsToLoad;
    }

    void
    SceneObject::setAnimationInstance
    (AnimationInstance* animationAsset)
    {
        mAnimationInstance.reset(animationAsset);
    }

    AnimationInstance*
    SceneObject::getAnimationInstance
    ()
    {
        return mAnimationInstance.get();
    }

    void
    SceneObject::setAudioInstance
    (AudioInstance* audioAsset)
    {
        mAudioInstance.reset(audioAsset);
    }

    AudioInstance*
    SceneObject::getAudioInstance
    ()
    {
        return mAudioInstance.get();
    }

    void
    SceneObject::setModelInstance
    (AssimpModelInstance* modelAsset)
    {
        mModelInstance.reset(modelAsset);
    }

    AssimpModelInstance*
    SceneObject::getModelInstance
    ()
    {
        return mModelInstance.get();
    }

    void
    SceneObject::setScriptInstance
    (LuaScriptInstance* scriptAsset)
    {
        mScriptInstance.reset(scriptAsset);
    }

    LuaScriptInstance*
    SceneObject::getScriptInstance
    ()
    {
        return mScriptInstance.get();
    }

    void
    SceneObject::setShaderInstance
    (ShaderInstance* shaderAsset)
    {
        mShaderInstance.reset(shaderAsset);
    }

    ShaderInstance*
    SceneObject::getShaderInstance
    ()
    {
        return mShaderInstance.get();
    }

    void
    SceneObject::setLightInstance
    (LightInstance* lightAsset)
    {
        mLightInstance.reset(lightAsset);
    }

    LightInstance*
    SceneObject::getLightInstance
    ()
    {
        return mLightInstance.get();
    }

    bool
    SceneObject::hasLightInstance
    ()
    {
        return mLightInstance != nullptr;
    }

    bool
    SceneObject::hasModelInstance
    ()
    {
        return mModelInstance != nullptr;
    }

    bool
    SceneObject::hasShaderInstance
    ()
    {
        return mShaderInstance != nullptr;
    }

    bool
    SceneObject::hasScriptInstance
    ()
    {
        return mScriptInstance != nullptr;
    }

    bool
    SceneObject::hasSpriteInstance
    ()
    {
        return mSpriteInstance != nullptr;
    }

    bool
    SceneObject::hasFontInstance
    ()
    {
        return mFontInstance != nullptr;
    }

    void
    SceneObject::setFontInstance
    (FontInstance* font)
    {
        mFontInstance.reset(font);
    }

    FontInstance*
    SceneObject::getFontInstance
    ()
    {
        return mFontInstance.get();
    }

    void
    SceneObject::setPhysicsObjectInstance
    (PhysicsObjectInstance* poi)
    {
        return mPhysicsObjectInstance.reset(poi);
    }

    PhysicsObjectInstance*
    SceneObject::getPhysicsObjectInstance
    ()
    {
        return mPhysicsObjectInstance.get();
    }

    string
    SceneObject::getTransformType
    ()
    {
        return mTransform->getTransformType();
    }

    void
    SceneObject::setTransformType
    (string transformType)
    {
        mTransform->setTransformType(transformType);
    }

    Transform3D*
    SceneObject::getTransform
    () {
        return mTransform.get();
    }

    void
    SceneObject::setTransform
    (Transform3D* transform)
    {
        mTransform.reset(transform);
    }

    void
    SceneObject::setParent
    (SceneObject* parent)
    {
        mParentHandle = parent;
    }

    SceneObject*
    SceneObject::getParent
    ()
    {
        return mParentHandle;
    }

    void SceneObject::setSpriteInstance(SpriteInstance* spriteAsset)
    {
        mSpriteInstance.reset(spriteAsset);
    }

    SpriteInstance*
    SceneObject::getSpriteInstance
    ()
    {
        return mSpriteInstance.get();
    }

    void
    SceneObject::setHasFocus
    (bool focus)
    {
        mHasFocus = focus;
    }

    bool
    SceneObject::hasFocus
    ()
    {
        return mHasFocus;
    }

    void
    SceneObject::addAssetDefUuidToLoad
    (string uuid)
    {
        mAssetDefUuidsToLoad.push_back(uuid);
    }

    void
    SceneObject::setDeleteFlag
    (bool del)
    {
        mDelete = del;
    }

    bool
    SceneObject::getDeleteFlag
    ()
    {
        return mDelete;
    }

    bool
    SceneObject::getLoadedFlag
    ()
    {
        return mLoaded;
    }

    void
    SceneObject::setLoadedFlag
    (bool loaded)
    {
        mLoaded = loaded;
    }

    SceneObject*
    SceneObject::getChildByUuid
    (string childUuid)
    {
        for (SceneObject* it : mChildren)
        {
            if (it->hasUuid(childUuid))
            {
                return it;
            }
        }
        return nullptr;
    }

    bool
    SceneObject::hasEvents
    ()
    {
        return mEventQueue.size() != 0;
    }

    void
    SceneObject::sendEvent
    (Event* event)
    {
        mEventQueue.push_back(event);
    }

    vector<Event*>*
    SceneObject::getEventQueue
    ()
    {
        return &mEventQueue;
    }

    void
    SceneObject::cleanUp
    ()
    {
        cleanUpEvents();
        deleteAssetInstances();
        setLoadedFlag(false);
        setDeleteFlag(false);
    }

    void
    SceneObject::cleanUpEvents
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Cleaning up events " << getNameAndUuidString()
                 << endl;
        }
        for (Event* it : mEventQueue)
        {
            delete it;
        }
        mEventQueue.clear();
    }

    bool
    SceneObject::hasPhysicsObjectInstance
    ()
    {
        return mPhysicsObjectInstance != nullptr;
    }

    vector<SceneObject*>
    SceneObject::getChildren
    ()
    {
        return mChildren;
    }

    nlohmann::json
    SceneObject::toJson
    ()
    {
        return mJson;
    }

    bool
    SceneObject::applyToAll
    (function<bool(SceneObject*)> funk)
    {
        bool retval = funk(this);

        for (SceneObject* it : mChildren)
        {
            if (it)
            {
                retval = retval || funk(it);
            }
        }
        return retval;
    }

    void*
    SceneObject::applyToAll
    (function<void*(SceneObject*)> funk)
    {
        void* retval = funk(this);
        if (retval)
        {
            return retval;
        }

        for (SceneObject* it : mChildren)
        {
            if (it)
            {
                retval = funk(it);
                if (retval)
                {
                    return retval;
                }
            }
        }
        return nullptr;
    }

    bool
    SceneObject::hasAnimationInstance
    ()
    {
        return mAnimationInstance != nullptr;
    }

    bool
    SceneObject::hasAudioInstance
    ()
    {
        return mAudioInstance != nullptr;
    }

    void
    SceneObject::createAssetInstances
    ()
    {
        for (string aDefUuid : mAssetDefUuidsToLoad)
        {
            createAssetInstanceFromAssetDefinitionByUuid(aDefUuid);
        }
    }

    void
    SceneObject::createAssetInstanceFromAssetDefinitionByUuid
    (string uuid)
    {
        AssetDefinition* assetDefinition =
            AssetDefinition::getAssetDefinitionByUuid(
                mScene->getProject()->getAssetDefinitions(),uuid
            );

        createAssetInstance(assetDefinition);
    }

    void
    SceneObject::createAssetInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Asset Intance of: ("
                 << definition->getType() << ") " << definition->getName()
                 << ", for  " << getNameAndUuidString()
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
    SceneObject::loadAssetInstances
    ()
    {
        string projectPath = mScene->getProject()->getProjectPath();

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
    SceneObject::createPhysicsObjectInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Physics Object Asset Instance." << endl;
        }
        mPhysicsObjectInstance.reset(
            new PhysicsObjectInstance(definition, mTransform.get(),
            mScene->getProject()->getAssetDefinitions())
        );
    }

    void
    SceneObject::createAnimationInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Animation asset instance." << endl;
        }
        mAnimationInstance.reset(new AnimationInstance(definition,mTransform.get()));
    }

    void
    SceneObject::createAudioInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Audio asset instance." << endl;
        }
        // hottest trainwreck 2017!
        mAudioInstance.reset(mScene
                ->getProject()
                ->getRuntime()
                ->getAudioComponent()
                ->newAudioInstance(definition,mTransform.get())
        );
    }

    void
    SceneObject::createModelInstance
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
    SceneObject::createScriptInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Script asset instance." << endl;
        }
        // hottest trainwreck 2017!
        mScriptInstance.reset(new LuaScriptInstance(definition, mTransform.get()));
        mScene->getProject()
              ->getRuntime()
              ->getLuaEngine()
              ->addToScriptMap(this,mScriptInstance.get());
    }

    void
    SceneObject::createShaderInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Shader asset instance." << endl;
        }
        mShaderInstance.reset(new ShaderInstance(definition,mTransform.get()));
    }

    void
    SceneObject::createLightInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Light Asset instance." << endl;
        }
        mLightInstance.reset(new LightInstance(definition,mTransform.get()));
    }

    void
    SceneObject::createSpriteInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Sprite Asset instance." << endl;
        }
        mSpriteInstance.reset(new SpriteInstance(definition,mTransform.get()));
    }

    void
    SceneObject::createFontInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Font Asset instance." << endl;
        }
        mFontInstance.reset(new FontInstance(definition,mTransform.get()));
    }


} // End of Dream
