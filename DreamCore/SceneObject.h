#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include "Transform3D.h"
#include "Constants.h"
#include "Event.h"
#include "Components/Animation/AnimationInstance.h"
#include "Components/Audio/IAudioInstance.h"
#include "Components/Graphics/AssimpModelInstance.h"
#include "Components/Graphics/LightInstance.h"
#include "Components/Graphics/ShaderInstance.h"
#include "Components/Graphics/SpriteInstance.h"
#include "Components/Graphics/FontInstance.h"
#include "Components/Physics/PhysicsObjectInstance.h"
#include "Lua/LuaScriptInstance.h"
#include "Lua/ILuaExposable.h"

using namespace std;
namespace Dream
{
  class SceneObject : public ILuaExposable
  {
  protected:
    vector<Event*> mEventQueue;
    bool mLoaded;
    SceneObject* mParent;
    vector<SceneObject*> mChildren;
    vector<string> mAssetDefUuidsToLoad;
    bool mHasFocus;
    // Metadata
    string mUuid;
    string mName;
    Transform3D *mTransform;
    bool mDelete;
    // Asset Instances
    IAudioInstance *mAudioInstance;
    AnimationInstance *mAnimationInstance;
    AssimpModelInstance *mModelInstance;
    ShaderInstance *mShaderInstance;
    LightInstance *mLightInstance;
    SpriteInstance *mSpriteInstance;
    LuaScriptInstance *mScriptInstance;
    PhysicsObjectInstance *mPhysicsObjectInstance;
    FontInstance *mFontInstance;
  public:
    SceneObject(nlohmann::json);
    SceneObject();
    ~SceneObject();
    void deleteChildren();
    void deleteAssetInstances();

    void constructorInit();

    bool init();
    void loadJsonAssetInstances(nlohmann::json);
    bool initAssetInstances();

    bool hasUuid(string);
    void setUuid(string);
    string getUuid();

    bool hasName(string);
    void setName(string);
    string getName();

    string getNameUuidString();

    void showStatus();

    glm::vec3 getTranslation();
    void setTranslation(float, float, float);
    void setTranslation(glm::vec3);
    void resetTranslation();

    glm::vec3 getRotation();
    void setRotation(float, float, float);
    void setRotation(glm::vec3);
    void resetRotation();

    glm::vec3 getScale();
    void setScale(float, float, float);
    void setScale(glm::vec3);
    void resetScale();

    void resetTransform();

    SceneObject* getChildByUuid(string);
    int countAllChildren();
    size_t countChildren();
    void addChild(SceneObject*);
    void removeChild(SceneObject*);
    bool isChildOf(SceneObject*);
    bool isChildOfDeep(SceneObject*);
    void getChildrenVectorDeep(vector<SceneObject*>*);
    bool isParentOf(SceneObject*);
    bool isParentOfDeep(SceneObject*);
    void setParent(SceneObject*);
    SceneObject* getParent();

    void setAnimationInstance(AnimationInstance*);
    AnimationInstance* getAnimationInstance();

    void setAudioInstance(IAudioInstance*);
    IAudioInstance* getAudioInstance();

    void setModelInstance(AssimpModelInstance*);
    AssimpModelInstance* getModelInstance();
    bool hasModelInstance();

    void setScriptInstance(LuaScriptInstance*);
    LuaScriptInstance* getScriptInstance();
    bool hasScriptInstance();

    void setShaderInstance(ShaderInstance*);
    ShaderInstance* getShaderInstance();
    bool hasShaderInstance();

    void setPhysicsObjectInstance(PhysicsObjectInstance*);
    PhysicsObjectInstance* getPhysicsObjectInstance();
    bool hasPhysicsObjectInstance();

    void setLightInstance(LightInstance*);
    LightInstance* getLightInstance();
    bool hasLightInstance();

    void setSpriteInstance(SpriteInstance*);
    SpriteInstance* getSpriteInstance();
    bool hasSpriteInstance();

    void setFontInstance(FontInstance*);
    FontInstance* getFontInstance();
    bool hasFontInstance();

    void addAssetDefUuidToLoad(string);
    vector<string> getAssetDefUuidsToLoad();

    string getTransformType();
    void setTransformType(string);

    Transform3D* getTransform();
    void setTransform(Transform3D*);

    bool hasFocus();
    void setHasFocus(bool);

    void copyTransform(Transform3D*);

    void setDeleteFlag(bool);
    bool getDeleteFlag();

    bool getLoadedFlag();
    void setLoadedFlag(bool);

    bool hasEvents();
    void sendEvent(Event*);
    vector<Event*>* getEventQueue();
    void cleanupEvents();
    void exposeLuaApi(lua_State*);

  }; // End of SceneObject

} // End of Dream

#endif // NODE_H
