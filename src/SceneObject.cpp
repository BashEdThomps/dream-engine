#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <algorithm>
#include "SceneObject.h"
#include "String.h"
#include "Uuid.h"

namespace Dream {

  SceneObject::SceneObject() {
    constructorInit();
    mUuid = Uuid::generateUuid();
  }

  SceneObject::SceneObject(nlohmann::json soJson){
    constructorInit();

    if(!soJson[SCENE_OBJECT_UUID].is_null()){
      mUuid = soJson[SCENE_OBJECT_UUID];
    }

    if (!soJson[SCENE_OBJECT_NAME].is_null()) {
      mName = soJson[SCENE_OBJECT_NAME];
    }

    if (!soJson[SCENE_OBJECT_TRANSFORM_TYPE].is_null()) {
      string transformType = soJson[SCENE_OBJECT_TRANSFORM_TYPE];
      mTransform->setTransformType(transformType);
    } else {
      string transformType = SCENE_OBJECT_TRANSFORM_TYPE_OFFSET;
      mTransform->setTransformType(transformType);
    }

    if (!soJson[SCENE_OBJECT_TRANSLATION].is_null()) {
      nlohmann::json translation = soJson[SCENE_OBJECT_TRANSLATION];
      setTranslation(translation[SCENE_OBJECT_X], translation[SCENE_OBJECT_Y], translation[SCENE_OBJECT_Z]);
    } else {
      resetTranslation();
    }

    if (!soJson[SCENE_OBJECT_ROTATION].is_null()) {
      nlohmann::json rotation = soJson[SCENE_OBJECT_ROTATION];
      setRotation(rotation[SCENE_OBJECT_X], rotation[SCENE_OBJECT_Y], rotation[SCENE_OBJECT_Z]);
    } else {
      resetRotation();
    }

    if (!soJson[SCENE_OBJECT_SCALE].is_null()) {
      nlohmann::json scale = soJson[SCENE_OBJECT_SCALE];
      setScale(scale[SCENE_OBJECT_X], scale[SCENE_OBJECT_Y], scale[SCENE_OBJECT_Z]);
    } else {
      resetScale();
    }

    if(!soJson[SCENE_OBJECT_ASSET_INSTANCES].is_null()) {
      loadJsonAssetInstances(soJson[SCENE_OBJECT_ASSET_INSTANCES]);
    }

    if(!soJson[SCENE_OBJECT_HAS_FOCUS].is_null()) {
      bool focus = soJson[SCENE_OBJECT_HAS_FOCUS];
      setHasFocus(focus);
    }
  }

  void SceneObject::constructorInit() {
    // Metadata
    mLoaded = false;
    mDelete = false;
    mHasFocus = false;
    mParent = nullptr;
    mTransform = new Transform3D();
    mChildren.clear();
    mAssetDefUuidsToLoad.clear();
    mUuid = "";
    mName = "";
    // Asset Instances
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

  void SceneObject::loadJsonAssetInstances(nlohmann::json assetInstancesJson) {
    for (nlohmann::json::iterator it = assetInstancesJson.begin(); it != assetInstancesJson.end(); it++) {
      mAssetDefUuidsToLoad.push_back((*it));
    }
  }

  void SceneObject::resetTransform() {
    resetTranslation();
    resetRotation();
    resetScale();
  }

  void SceneObject::resetTranslation() {
    setTranslation(0.0f, 0.0f, 0.0f);
  }

  void SceneObject::resetRotation() {
    setRotation(0.0f, 0.0f, 0.0f);
  }

  void SceneObject::resetScale() {
    setScale(1.0f, 1.0f, 1.0f);
  }

  SceneObject::~SceneObject() {
    deleteChildren();
    deleteAssetInstances();
  }

  void SceneObject::deleteChildren() {
    vector<SceneObject*>::iterator it;
    for (it=mChildren.begin(); it!=mChildren.end(); it++) {
      SceneObject* child = (*it);
      delete child;
    }
    mChildren.clear();
  }

  void SceneObject::deleteAssetInstances() {
    if (mTransform != nullptr) {
      delete mTransform;
      mTransform = nullptr;
    }
    if (mAudioInstance != nullptr) {
      delete mAudioInstance;
      mAudioInstance = nullptr;
    }
    if (mAnimationInstance != nullptr) {
      delete mAnimationInstance;
      mAnimationInstance = nullptr;
    }
    if (mModelInstance != nullptr) {
      delete mModelInstance;
      mModelInstance = nullptr;
    }
    if (mShaderInstance != nullptr) {
      delete mShaderInstance;
      mShaderInstance = nullptr;
    }
    if (mLightInstance != nullptr) {
      delete mLightInstance;
      mLightInstance = nullptr;
    }
    if (mSpriteInstance != nullptr) {
      delete mSpriteInstance;
      mSpriteInstance = nullptr;
    }
    if (mScriptInstance != nullptr) {
      delete mScriptInstance;
      mScriptInstance = nullptr;
    }
    if (mPhysicsObjectInstance != nullptr) {
      delete mPhysicsObjectInstance;
      mPhysicsObjectInstance = nullptr;
    }
    if (mFontInstance != nullptr) {
      delete mFontInstance;
      mFontInstance = nullptr;
    }
  }

  bool SceneObject::hasName(string name) {
    return mName == name;
  }

  void SceneObject::setName(string name) {
    mName = name;
  }

  string SceneObject::getName() {
    return mName;
  }

  void SceneObject::setTranslation(vector<float> translation) {
    mTransform->setTranslation(translation);
  }

  void SceneObject::setRotation(vector<float> rotation) {
    mTransform->setRotation(rotation);
  }

  void SceneObject::setScale(vector<float> scale) {
    mTransform->setScale(scale);
  }

  void SceneObject::setTranslation(float x, float y, float z) {
    mTransform->setTranslation(x,y,z);
  }

  void SceneObject::setRotation(float x, float y, float z) {
    mTransform->setRotation(x,y,z);
  }

  void SceneObject::setScale(float x, float y, float z) {
    mTransform->setScale(x,y,z);
  }

  vector<float> SceneObject::getRotation() {
    return mTransform->getRotation();
  }

  vector<float> SceneObject::getScale() {
    return mTransform->getScale();
  }

  vector<float> SceneObject::getTranslation() {
    return mTransform->getTranslation();
  }

  bool SceneObject::hasUuid(string uuid) {
    return (mUuid.compare(uuid) == 0);
  }

  void SceneObject::setUuid(string uuid) {
    mUuid = uuid;
  }

  string SceneObject::getUuid() {
    return mUuid;
  }

  int  SceneObject::countAllChildren() {
    if (DEBUG) {
      cout << "SceneObject: Count All Children, Not Implemented" << endl;
    }
    return -1;
  }

  size_t  SceneObject::countChildren() {
    return mChildren.size();
  }

  void SceneObject::addChild(SceneObject* child) {
    child->setParent(this);
    mChildren.push_back(child);
  }

  void SceneObject::removeChild(SceneObject* child) {
    mChildren.erase(remove(mChildren.begin(), mChildren.end(), child), mChildren.end());
  }

  bool SceneObject::isChildOfDeep(SceneObject* parent) {
    return mParent == parent;
  }

  bool SceneObject::isChildOf(SceneObject* parent) {
    return mParent == parent;
  }

  bool SceneObject::isParentOf(SceneObject* sceneObject) {
    return sceneObject->getParent() == this;
  }

  bool SceneObject::isParentOfDeep(SceneObject* sceneObject) {
    return false;
  }

  string SceneObject::getNameUuidString() {
    return getName()+" ("+getUuid()+")";
  }

  void SceneObject::getChildrenVectorDeep(vector<SceneObject*>* soVector) {
    soVector->push_back(this);
    if (!mChildren.empty()) {
      vector<SceneObject*>::iterator it;
      for (it = mChildren.begin(); it != mChildren.end(); it++) {
        if ((*it) != nullptr) {
          (*it)->getChildrenVectorDeep(soVector);
        }
      }
    }
  }

  void SceneObject::showStatus() {

    if (DEBUG) {
      cout << "SceneObject:" << endl;
      cout << "          Uuid: " << mUuid << endl;
      cout << "          Name: " << mName << endl;

      if (mParent != nullptr) {
        cout << "    ParentUuid: " << mParent->getUuid() << endl;
      }

      cout << "      Children: " << mChildren.size() << endl;
      cout << "Trnasform Type: " << mTransform->getTransformType() << endl;
      cout << "   Translation: " << String::floatVectorToString(getTranslation()) << endl;
      cout << "      Rotation: " << String::floatVectorToString(getRotation())<< endl;
      cout << "         Scale: " << String::floatVectorToString(getScale())<< endl;
    }
  }

  vector<string> SceneObject::getAssetDefUuidsToLoad() {
    return mAssetDefUuidsToLoad;
  }

  void SceneObject::setAnimationInstance(AnimationInstance* animationAsset) {
    mAnimationInstance = animationAsset;
  }

  AnimationInstance* SceneObject::getAnimationInstance() {
    return mAnimationInstance;
  }

  void SceneObject::setAudioInstance(AudioInstance* audioAsset) {
    mAudioInstance = audioAsset;
  }

  AudioInstance* SceneObject::getAudioInstance() {
    return mAudioInstance;
  }

  void SceneObject::setModelInstance(AssimpModelInstance* modelAsset) {
    mModelInstance = modelAsset;
  }

  AssimpModelInstance* SceneObject::getModelInstance() {
    return mModelInstance;
  }

  void SceneObject::setScriptInstance(LuaScriptInstance* scriptAsset) {
    mScriptInstance = scriptAsset;
  }

  LuaScriptInstance* SceneObject::getScriptInstance() {
    return mScriptInstance;
  }

  void SceneObject::setShaderInstance(ShaderInstance* shaderAsset) {
    mShaderInstance = shaderAsset;
  }

  ShaderInstance* SceneObject::getShaderInstance() {
    return mShaderInstance;
  }

  void SceneObject::setLightInstance(LightInstance* lightAsset) {
    mLightInstance = lightAsset;
  }

  LightInstance* SceneObject::getLightInstance() {
    return mLightInstance;
  }

  bool SceneObject::hasModelInstance() {
    return mModelInstance != nullptr;
  }

  bool SceneObject::hasShaderInstance() {
    return mShaderInstance != nullptr;
  }

  bool SceneObject::hasScriptInstance() {
    return mScriptInstance != nullptr;
  }

  bool SceneObject::hasSpriteInstance() {
    return mSpriteInstance != nullptr;
  }

  bool SceneObject::hasFontInstance() {
    return mFontInstance != nullptr;
  }

  void SceneObject::setFontInstance(FontInstance* font) {
    mFontInstance = font;
  }

  FontInstance* SceneObject::getFontInstance() {
    return mFontInstance;
  }

  void SceneObject::setPhysicsObjectInstance(PhysicsObjectInstance* physicsObject) {
    mPhysicsObjectInstance = physicsObject;
  }

  PhysicsObjectInstance* SceneObject::getPhysicsObjectInstance() {
    return mPhysicsObjectInstance;
  }

  string SceneObject::getTransformType() {
    return mTransform->getTransformType();
  }

  void SceneObject::setTransformType(string transformType) {
    mTransform->setTransformType(transformType);
  }

  Transform3D* SceneObject::getTransform() {
    return mTransform;
  }

  void SceneObject::setTransform(Transform3D* transform) {
    mTransform = transform;
  }

  void SceneObject::setParent(SceneObject* parent) {
    mParent = parent;
  }

  SceneObject* SceneObject::getParent() {
    return mParent;
  }

  void SceneObject::setSpriteInstance(SpriteInstance* spriteAsset) {
    mSpriteInstance = spriteAsset;
  }

  SpriteInstance* SceneObject::getSpriteInstance() {
    return mSpriteInstance;
  }

  void SceneObject::setHasFocus(bool focus) {
    mHasFocus = focus;
  }

  bool SceneObject::hasFocus() {
    return mHasFocus;
  }

  void SceneObject::addAssetDefUuidToLoad(string uuid) {
    mAssetDefUuidsToLoad.push_back(uuid);
  }

  void SceneObject::copyTransform(Transform3D* transform) {
    if (mTransform != nullptr) {
      delete mTransform;
    }

    mTransform = new Transform3D(transform);
  }

  void SceneObject::setDeleteFlag(bool del) {
    mDelete = del;
  }

  bool SceneObject::getDeleteFlag() {
    return mDelete;
  }

  bool SceneObject::getLoadedFlag() {
    return mLoaded;
  }

  void SceneObject::setLoadedFlag(bool loaded) {
    mLoaded = loaded;
  }

  SceneObject* SceneObject::getChildByUuid(string childUuid) {
    vector<SceneObject*>::iterator it;
    for (it=mChildren.begin(); it!=mChildren.end(); it++) {
      if ((*it)->getUuid().compare(childUuid) == 0) {
        return (*it);
      }
    }
    return nullptr;
  }

  bool SceneObject::hasEvents() {
    return mEventQueue.size() != 0;
  }

  void SceneObject::sendEvent(Event* event) {
    mEventQueue.push_back(event);
  }

  vector<Event*>* SceneObject::getEventQueue() {
    return &mEventQueue;
  }

  void SceneObject::cleanupEvents() {
    vector<Event*>::iterator it;
    for (it=mEventQueue.begin();it!=mEventQueue.end();it++) {
      delete (*it);
    }
    mEventQueue.clear();
  }

  bool SceneObject::hasPhysicsObjectInstance() {
    return mPhysicsObjectInstance != nullptr;
  }
} // End of Dream
