#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include <functional>
#include <memory>

#include "Transform3D.h"
#include "Constants.h"
#include "Event.h"
#include "Components/Animation/AnimationInstance.h"
#include "Components/Audio/AudioInstance.h"
#include "Components/Graphics/AssimpModelInstance.h"
#include "Components/Graphics/LightInstance.h"
#include "Components/Graphics/ShaderInstance.h"
#include "Components/Graphics/SpriteInstance.h"
#include "Components/Graphics/FontInstance.h"
#include "Components/Physics/PhysicsObjectInstance.h"
#include "Lua/LuaScriptInstance.h"

using namespace std;
namespace Dream
{
  class SceneObject;

  class SceneObject
  {
  protected:
    // Metadata
    nlohmann::json mJson;
    string mUuid;
    shared_ptr<SceneObject> mParent;
    vector<SceneObject> mChildren;
    vector<string> mAssetDefUuidsToLoad;
    bool mHasFocus;
    string mName;
    Transform3D mInitialTransform;

    // Runtime Data
    vector<Event> mEventQueue;
    bool mLoaded;
    bool mDelete;
    Transform3D mCurrentTransform;

    // Asset Instance Pointers
    shared_ptr<AudioInstance> mAudioInstance;
    shared_ptr<AnimationInstance> mAnimationInstance;
    shared_ptr<FontInstance> mFontInstance;
    shared_ptr<LightInstance> mLightInstance;
    shared_ptr<AssimpModelInstance> mModelInstance;
    shared_ptr<PhysicsObjectInstance> mPhysicsObjectInstance;
    shared_ptr<LuaScriptInstance> mScriptInstance;
    shared_ptr<ShaderInstance> mShaderInstance;
    shared_ptr<SpriteInstance> mSpriteInstance;

  public:
    SceneObject(nlohmann::json);
    SceneObject();

    bool operator==(SceneObject);
    SceneObject operator=(SceneObject);
    inline bool operator< (const SceneObject& rhs) const { return this->mUuid < rhs.mUuid;    }
    inline bool operator> (const SceneObject& rhs) const { return rhs.mUuid < this->mUuid;    }
    inline bool operator<=(const SceneObject& rhs) const { return !(this->mUuid > rhs.mUuid); }
    inline bool operator>=(const SceneObject& rhs) const { return !(this->mUuid < rhs.mUuid); }

    void constructorInit();

    ~SceneObject();
    void clearChildren();

    bool init();
    void loadMetadata(nlohmann::json);
    void loadJsonAssetInstances(nlohmann::json);
    bool initAssetInstances();

    bool hasUuid(string);
    void setUuid(string);
    string getUuid();

    bool hasName(string);
    void setName(string);
    string getName();

    string getNameAndUuidString();

    void showStatus();

    SceneObject getChildByUuid(string);
    int countAllChildren();
    size_t countChildren();
    void addChild(SceneObject&);
    void removeChild(SceneObject);
    bool isChildOf(SceneObject&);

    vector<SceneObject> getChildren();
    bool isParentOf(SceneObject&);

    void setParent(shared_ptr<SceneObject>);
    shared_ptr<SceneObject> getParent();

    void setAnimationInstance(shared_ptr<AnimationInstance>);
    shared_ptr<AnimationInstance> getAnimationInstance();

    void setAudioInstance(shared_ptr<AudioInstance>);
    shared_ptr<AudioInstance> getAudioInstance();

    void setModelInstance(shared_ptr<AssimpModelInstance>);
    shared_ptr<AssimpModelInstance> getModelInstance();
    bool hasModelInstance();

    void setScriptInstance(shared_ptr<LuaScriptInstance>);
    shared_ptr<LuaScriptInstance> getScriptInstance();
    bool hasScriptInstance();

    void setShaderInstance(shared_ptr<ShaderInstance>);
    shared_ptr<ShaderInstance> getShaderInstance();
    bool hasShaderInstance();

    void setPhysicsObjectInstance(shared_ptr<PhysicsObjectInstance>);
    shared_ptr<PhysicsObjectInstance> getPhysicsObjectInstance();
    bool hasPhysicsObjectInstance();

    void setLightInstance(shared_ptr<LightInstance>);
    shared_ptr<LightInstance> getLightInstance();
    bool hasLightInstance();

    void setSpriteInstance(shared_ptr<SpriteInstance>);
    shared_ptr<SpriteInstance> getSpriteInstance();
    bool hasSpriteInstance();

    void setFontInstance(shared_ptr<FontInstance>);
    shared_ptr<FontInstance> getFontInstance();
    bool hasFontInstance();

    void addAssetDefUuidToLoad(string);
    vector<string> getAssetDefUuidsToLoad();

    Transform3D getCurrentTransform();
    void setCurrentTransform(Transform3D);

    Transform3D getInitialTransform();
    void setInitialTransform(Transform3D);

    bool hasFocus();
    void setHasFocus(bool);

    void setDeleteFlag(bool);
    bool getDeleteFlag();

    bool getLoadedFlag();
    void setLoadedFlag(bool);

    bool hasEvents();
    void sendEvent(Event&);
    vector<Event> getEventQueue();
    void cleanupEvents();
    nlohmann::json toJson();

    void applyToAll(function<void(SceneObject&)> function);
    SceneObject applyToAll(function<SceneObject(SceneObject&)> function);
    bool applyToAll(function<bool(SceneObject&)> function);

  }; // End of SceneObject

} // End of Dream

#endif // NODE_H
