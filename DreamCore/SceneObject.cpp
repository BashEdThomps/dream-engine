#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <algorithm>
#include "SceneObject.h"
#include "String.h"
#include "Uuid.h"

namespace Dream
{

    SceneObject::SceneObject
    ()
    {
        constructorInit();
        mUuid = Uuid::generateUuid();
    }

    SceneObject::SceneObject
    (nlohmann::json soJson)
    {
        mJson = soJson;
        constructorInit();
        loadMetadata(soJson);
    }

    bool
    SceneObject::operator==
    (SceneObject other)
    {
        return mUuid.compare(other.mUuid) == 0;
    }

    SceneObject
    SceneObject::operator=
    (SceneObject obj)
    {
        // Metadata
        mJson                = obj.mJson;
        mUuid                = obj.mUuid;
        mParent              = obj.mParent;
        mChildren            = obj.mChildren;
        mAssetDefUuidsToLoad = obj.mAssetDefUuidsToLoad;
        mHasFocus            = obj.mHasFocus;
        mName                = obj.mName;
        mInitialTransform    = obj.mInitialTransform;
        // Runtime Data
        mEventQueue          = obj.mEventQueue;
        mLoaded              = obj.mLoaded;
        mDelete              = obj.mDelete;
        mCurrentTransform    = obj.mCurrentTransform;
        // Asset Instance Pointers
        mAudioInstance       = obj.mAudioInstance;
        mAnimationInstance   = obj.mAnimationInstance;
        mFontInstance        = obj.mFontInstance;
        mLightInstance       = obj.mLightInstance;
        mModelInstance       = obj.mModelInstance;
        mPhysicsObjectInstance = obj.mPhysicsObjectInstance;
        mScriptInstance      = obj.mScriptInstance;
        mShaderInstance      = obj.mShaderInstance;
        mSpriteInstance      = obj.mSpriteInstance;
        return *this;
    }

    void
    SceneObject::constructorInit
    ()
    {
        mLoaded = false;
        mDelete = false;
        mHasFocus = false;
        mParent = nullptr;
    }

    void
    SceneObject::loadMetadata
    (nlohmann::json soJson)
    {
        if(!soJson[SCENE_OBJECT_UUID].is_null())
        {
            mUuid = soJson[SCENE_OBJECT_UUID];
        }

        if (!soJson[SCENE_OBJECT_NAME].is_null())
        {
            mName = soJson[SCENE_OBJECT_NAME];
        }

        if (!soJson[SCENE_OBJECT_TRANSFORM_TYPE].is_null())
        {
            string transformType = soJson[SCENE_OBJECT_TRANSFORM_TYPE];
            mInitialTransform.setTransformType(transformType);
        }
        else
        {
            string transformType = SCENE_OBJECT_TRANSFORM_TYPE_ABSOLUTE;
            mInitialTransform.setTransformType(transformType);
        }

        if (!soJson[SCENE_OBJECT_TRANSLATION].is_null())
        {
            nlohmann::json translation = soJson[SCENE_OBJECT_TRANSLATION];
            mInitialTransform.setTranslation(
                translation[SCENE_OBJECT_X],
                translation[SCENE_OBJECT_Y],
                translation[SCENE_OBJECT_Z]
            );
        }
        else
        {
            mInitialTransform.resetTranslation();
        }

        if (!soJson[SCENE_OBJECT_ROTATION].is_null())
        {
            nlohmann::json rotation = soJson[SCENE_OBJECT_ROTATION];
            mInitialTransform.setRotation(
                rotation[SCENE_OBJECT_X],
                rotation[SCENE_OBJECT_Y],
                rotation[SCENE_OBJECT_Z]
            );
        }
        else
        {
            mInitialTransform.resetOrientation();
        }

        if (!soJson[SCENE_OBJECT_SCALE].is_null())
        {
            nlohmann::json scale = soJson[SCENE_OBJECT_SCALE];
            mInitialTransform.setScale(
                scale[SCENE_OBJECT_X],
                scale[SCENE_OBJECT_Y],
                scale[SCENE_OBJECT_Z]
            );
        }
        else
        {
            mInitialTransform.resetScale();
        }

        if(!soJson[SCENE_OBJECT_ASSET_INSTANCES].is_null())
        {
            loadJsonAssetInstances(soJson[SCENE_OBJECT_ASSET_INSTANCES]);
        }

        if(!soJson[SCENE_OBJECT_HAS_FOCUS].is_null())
        {
            bool focus = soJson[SCENE_OBJECT_HAS_FOCUS];
            setHasFocus(focus);
        }
    }

    void
    SceneObject::loadJsonAssetInstances
    (nlohmann::json assetInstancesJson)
    {
        for (nlohmann::json it : assetInstancesJson)
        {
            mAssetDefUuidsToLoad.push_back(it);
        }
    }

    SceneObject::~SceneObject
    ()
    {
        if (DEBUG)
        {
            cout << "SceneObject: Destroying Object "
                 << getNameAndUuidString() << endl;
        }
    }

    void
    SceneObject::clearChildren
    ()
    {
        mChildren.clear();
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

    size_t
    SceneObject::countChildren
    ()
    {
        return mChildren.size();
    }

    void
    SceneObject::addChild
    (SceneObject& child)
    {
        child.setParent(shared_ptr<SceneObject>(this));
        mChildren.push_back(child);
    }

    void
    SceneObject::removeChild
    (SceneObject child)
    {
        mChildren.erase(remove(mChildren.begin(), mChildren.end(), child), mChildren.end());
    }

    bool
    SceneObject::isChildOf
    (SceneObject& parent)
    {
        // TODO, check this
        return mParent.get() == &parent;
    }

    bool
    SceneObject::isParentOf
    (SceneObject& sceneObject)
    {
        // TODO, check this
        return sceneObject.getParent().get() == this;
    }

    string
    SceneObject::getNameAndUuidString
    ()
    {
        return getName()+" ("+getUuid()+")";
    }

    void
    SceneObject::applyToAll
    (function<void(SceneObject&)> funk)
    {
        if (DEBUG)
        {
            cout << "SceneObejct: applyToAll void(SceneObject) called on " << getNameAndUuidString()
                 << endl;
        }

        funk(*this);

        for (SceneObject so : mChildren)
        {
            so.applyToAll(funk);
        }
    }

    bool
    SceneObject::applyToAll
    (function<bool(SceneObject&)> funk)
    {
        bool retval;
        if (DEBUG)
        {
            cout << "SceneObejct: applyToAll SceneObject(SceneObject) called on " << getNameAndUuidString()
                 << endl;
        }

        retval = funk(*this);
        if (!retval)
        {
            return retval;
        }

        for (SceneObject so : mChildren)
        {
            retval = so.applyToAll(funk);
            if (!retval)
            {
                return retval;
            }
        }

        return retval;
    }

    SceneObject
    SceneObject::applyToAll
    (function<SceneObject(SceneObject&)> funk)
    {
        SceneObject retval;
        if (DEBUG)
        {
            cout << "SceneObejct: applyToAll SceneObject(SceneObject) called on " << getNameAndUuidString()
                 << endl;
        }

        retval = funk(*this);

        for (SceneObject so : mChildren)
        {
            retval = so.applyToAll(funk);
        }

        return retval;
    }

    void
    SceneObject::showStatus
    ()
    {
        cout << "SceneObject: " << getNameAndUuidString() << endl;
        if (mParent != nullptr)
        {
            cout << "    ParentUuid: " << mParent->getUuid() << endl;
        }
        cout << "      Children: " << mChildren.size() << endl;
        cout << "Trnasform Type: " << mInitialTransform.getTransformType() << endl;
        cout << "   Translation: " << String::vec3ToString(mInitialTransform.getTranslation()) << endl;
        cout << "      Rotation: " << String::vec3ToString(mInitialTransform.getRotation())<< endl;
        cout << "         Scale: " << String::vec3ToString(mInitialTransform.getScale())<< endl;
    }

    vector<string>
    SceneObject::getAssetDefUuidsToLoad
    ()
    {
        return mAssetDefUuidsToLoad;
    }

    void
    SceneObject::setAnimationInstance
    (shared_ptr<AnimationInstance> animationAsset)
    {
        mAnimationInstance = animationAsset;
    }

    shared_ptr<AnimationInstance>
    SceneObject::getAnimationInstance
    ()
    {
        return mAnimationInstance;
    }

    void
    SceneObject::setAudioInstance
    (shared_ptr<AudioInstance> audioAsset)
    {
        mAudioInstance = audioAsset;
    }

    shared_ptr<AudioInstance>
    SceneObject::getAudioInstance
    ()
    {
        return mAudioInstance;
    }

    void
    SceneObject::setModelInstance
    (shared_ptr<AssimpModelInstance> modelAsset)
    {
        mModelInstance = modelAsset;
    }

    shared_ptr<AssimpModelInstance>
    SceneObject::getModelInstance
    ()
    {
        return mModelInstance;
    }

    void
    SceneObject::setScriptInstance
    (shared_ptr<LuaScriptInstance> scriptAsset)
    {
        mScriptInstance = scriptAsset;
    }

    shared_ptr<LuaScriptInstance>
    SceneObject::getScriptInstance
    ()
    {
        return mScriptInstance;
    }

    void
    SceneObject::setShaderInstance
    (shared_ptr<ShaderInstance> shaderAsset)
    {
        mShaderInstance = shaderAsset;
    }

    shared_ptr<ShaderInstance>
    SceneObject::getShaderInstance
    ()
    {
        return mShaderInstance;
    }

    void
    SceneObject::setLightInstance
    (shared_ptr<LightInstance> lightAsset)
    {
        mLightInstance = lightAsset;
    }

    shared_ptr<LightInstance>
    SceneObject::getLightInstance
    ()
    {
        return mLightInstance;
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
    (shared_ptr<FontInstance> font)
    {
        mFontInstance = font;
    }

    shared_ptr<FontInstance>
    SceneObject::getFontInstance
    ()
    {
        return mFontInstance;
    }

    void
    SceneObject::setPhysicsObjectInstance
    (shared_ptr<PhysicsObjectInstance> physicsObject)
    {
        mPhysicsObjectInstance = physicsObject;
    }

    shared_ptr<PhysicsObjectInstance>
    SceneObject::getPhysicsObjectInstance
    ()
    {
        return mPhysicsObjectInstance;
    }

    Transform3D
    SceneObject::getInitialTransform
    ()
    {
        return mInitialTransform;
    }

    Transform3D
    SceneObject::getCurrentTransform
    ()
    {
        return mCurrentTransform;
    }

    void
    SceneObject::setInitialTransform
    (Transform3D transform)
    {
        mInitialTransform = transform;
    }

    void
    SceneObject::setCurrentTransform
    (Transform3D transform)
    {
        mCurrentTransform = transform;
    }

    void
    SceneObject::setParent
    (shared_ptr<SceneObject> parent)
    {
        mParent = parent;
    }

    shared_ptr<SceneObject>
    SceneObject::getParent
    ()
    {
        return mParent;
    }

    void
    SceneObject::setSpriteInstance
    (shared_ptr<SpriteInstance> spriteAsset)
    {
        mSpriteInstance = spriteAsset;
    }

    shared_ptr<SpriteInstance>
    SceneObject::getSpriteInstance
    ()
    {
        return mSpriteInstance;
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
    SceneObject::setDeleteFlag(bool del)
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

    SceneObject
    SceneObject::getChildByUuid
    (string childUuid)
    {
        SceneObject retval;
        for (SceneObject it : mChildren)
        {
            if (it.getUuid().compare(childUuid) == 0)
            {
                retval = it;
                break;
            }
        }
        return retval;
    }

    bool
    SceneObject::hasEvents
    ()
    {
        return mEventQueue.size() != 0;
    }

    void
    SceneObject::sendEvent
    (Event& event)
    {
        mEventQueue.push_back(event);
    }

    vector<Event>
    SceneObject::getEventQueue
    ()
    {
        return mEventQueue;
    }

    void
    SceneObject::cleanupEvents
    ()
    {
        mEventQueue.clear();
    }

    bool
    SceneObject::hasPhysicsObjectInstance
    ()
    {
        return mPhysicsObjectInstance != nullptr;
    }

    vector<SceneObject>
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
} // End of Dream
