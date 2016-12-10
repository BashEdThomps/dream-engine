#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <algorithm>
#include "SceneObject.h"
#include "String.h"

namespace Dream {

  SceneObject::SceneObject() {
    mHasFocus = false;
    mParent = nullptr;
    mTransform = new Transform3D();
  }

  SceneObject::SceneObject(nlohmann::json soJson){
    mHasFocus = false;
    mParent = nullptr;
    mTransform = new Transform3D();

    if(!soJson[SCENE_OBJECT_UUID].is_null()){
      mUUID = soJson[SCENE_OBJECT_UUID];
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
      loadAssetInstances(soJson[SCENE_OBJECT_ASSET_INSTANCES]);
    }

    if(!soJson[SCENE_OBJECT_HAS_FOCUS].is_null()) {
      bool focus = soJson[SCENE_OBJECT_HAS_FOCUS];
      setHasFocus(focus);
    }

  }

  void SceneObject::loadAssetInstances(nlohmann::json assetInstancesJson) {
    mAnimationInstance = nullptr;
    mAudioInstance = nullptr;
    mModelInstance = nullptr;
    mScriptInstance = nullptr;
    mLightInstance = nullptr;
    mSpriteInstance = nullptr;
    mFontInstance = nullptr;

    for (nlohmann::json::iterator it = assetInstancesJson.begin(); it != assetInstancesJson.end(); it++) {
      mAssetInstanceUUIDsToLoad.push_back((*it));
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
    // Pass
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

  bool SceneObject::hasUUID(string uuid) {
    return (mUUID.compare(uuid) == 0);
  }

  void SceneObject::setUUID(string uuid) {
    mUUID = uuid;
  }

  string SceneObject::getUUID() {
    return mUUID;
  }

  int  SceneObject::countAllChildren() {
    return -1;
  }

  size_t  SceneObject::countChildren() {
    return mChildren.size();
  }

  void SceneObject::addChild(SceneObject* child) {
    mChildren.push_back(child);
  }

  void SceneObject::removeChild(SceneObject* child) {

  }

  bool SceneObject::isChildOfDeep(SceneObject* parent) {
    return mParent == parent;
  }

  bool SceneObject::isChildOf(SceneObject* parent) {
    return mParent == parent;
  }

  bool SceneObject::isParentOf(SceneObject* sceneObject) {
    return false;
  }

  bool SceneObject::isParentOfDeep(SceneObject* sceneObject) {
    return false;
  }

  string SceneObject::getNameUUIDString() {
    return getName()+" ("+getUUID()+")";
  }

  void SceneObject::getChildrenVectorDeep(vector<SceneObject*>* soVector) {
    soVector->push_back(this);
    for (vector<SceneObject*>::iterator it = mChildren.begin(); it != mChildren.end(); it++) {
      (*it)->getChildrenVectorDeep(soVector);
    }
  }

  void SceneObject::generatePath() {
    mPath = "";
    SceneObject* current = this;
    while (current != nullptr) {
      mPath = current->getUUID()+mPath;
      current = current->getParent();
      if (current != nullptr) {
        mPath = PATH_DELIMETER+mPath;
      }
    }
  }

  string SceneObject::getPath() {
    return mPath;
  }

  void SceneObject::showStatus() {
    cout << "SceneObject:" << endl;
    cout << "          UUID: " << mUUID << endl;
    cout << "          Name: " << mName << endl;

    if (mParent != nullptr) {
        cout << "    ParentUUID: " << mParent->getUUID() << endl;
    }

    cout << "      Children: " << mChildren.size() << endl;
    cout << "Trnasform Type: " << mTransform->getTransformType() << endl;
    cout << "   Translation: " << String::floatVectorToString(getTranslation()) << endl;
    cout << "      Rotation: " << String::floatVectorToString(getRotation())<< endl;
    cout << "         Scale: " << String::floatVectorToString(getScale())<< endl;
  }

  vector<string> SceneObject::getAssetInstanceUUIDsToLoad() {
    return mAssetInstanceUUIDsToLoad;
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

} // End of Dream
