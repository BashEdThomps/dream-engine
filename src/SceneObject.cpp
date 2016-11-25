#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <algorithm>
#include "SceneObject.h"
#include "String.h"

namespace Dream {

  SceneObject::SceneObject() {
    mParent = nullptr;
  }

  SceneObject::SceneObject(nlohmann::json soJson){
    mTransform = new Transform3D();

    if(!soJson[SCENE_OBJECT_UUID].is_null()){
      mUUID = soJson[SCENE_OBJECT_UUID];
    }

    if (!soJson[SCENE_OBJECT_NAME].is_null()) {
      mName = soJson[SCENE_OBJECT_NAME];
    }

    if (!soJson[SCENE_OBJECT_TRANSFORM_TYPE].is_null()) {
      mTransformType = soJson[SCENE_OBJECT_TRANSFORM_TYPE];
    } else {
      mTransformType = SCENE_OBJECT_TRANSFORM_TYPE_OFFSET;
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
  }

  void SceneObject::loadAssetInstances(nlohmann::json assetInstancesJson) {

    mAnimationAssetInstance = nullptr;
    mAudioAssetInstance     = nullptr;
    mModelAssetInstance     = nullptr;
    mScriptAssetInstance    = nullptr;

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

  SceneObject::~SceneObject() {}

  bool SceneObject::hasName(std::string name) {
    return mName == name;
  }

  void SceneObject::setName(std::string name) {
    mName = name;
  }

  std::string SceneObject::getName() {
    return mName;
  }

  void SceneObject::setTranslation(std::vector<float> translation) {
    mTransform->setTranslation(translation);
  }

  void SceneObject::setRotation(std::vector<float> rotation) {
    mTransform->setRotation(rotation);
  }

  void SceneObject::setScale(std::vector<float> scale) {
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

  std::vector<float> SceneObject::getRotation() {
    return mTransform->getRotation();
  }

  std::vector<float> SceneObject::getScale() {
    return mTransform->getScale();
  }

  std::vector<float> SceneObject::getTranslation() {
    return mTransform->getTranslation();
  }

  bool SceneObject::hasUUID(std::string uuid) {
    return (mUUID.compare(uuid) == 0);
  }

  void SceneObject::setUUID(std::string uuid) {
    mUUID = uuid;
  }

  std::string SceneObject::getUUID() {
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

  void SceneObject::removeChild(SceneObject* child) {}

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

  std::string SceneObject::getNameUUIDString() {
    return getName()+" ("+getUUID()+")";
  }

  void SceneObject::getChildrenVectorDeep(std::vector<SceneObject*>* vector) {
    vector->push_back(this);
    for (std::vector<SceneObject*>::iterator it = mChildren.begin(); it != mChildren.end(); it++) {
      (*it)->getChildrenVectorDeep(vector);
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

  std::string SceneObject::getPath() {
    return mPath;
  }

  void SceneObject::showStatus() {
    std::cout << "SceneObject:" << std::endl;
    std::cout << "          UUID: " << mUUID << std::endl;
    std::cout << "          Name: " << mName << std::endl;

    if (mParent != nullptr) {
        std::cout << "    ParentUUID: " << mParent->getUUID() << std::endl;
    }

    std::cout << "      Children: " << mChildren.size() << std::endl;
    std::cout << "Trnasform Type: " << mTransformType << std::endl;
    std::cout << "   Translation: " << String::floatVectorToString(getTranslation()) << std::endl;
    std::cout << "      Rotation: " << String::floatVectorToString(getRotation())<< std::endl;
    std::cout << "         Scale: " << String::floatVectorToString(getScale())<< std::endl;
  }

  std::vector<std::string> SceneObject::getAssetInstanceUUIDsToLoad() {
    return mAssetInstanceUUIDsToLoad;
  }

  void SceneObject::setAnimationAssetInstance(AssetInstance* animationAsset) {
    mAnimationAssetInstance = animationAsset;
  }

  AssetInstance* SceneObject::getAnimationAssetInstance() {
    return mAnimationAssetInstance;
  }

  void SceneObject::setAudioAssetInstance(AssetInstance* audioAsset) {
    mAudioAssetInstance = audioAsset;
  }

  AssetInstance* SceneObject::getAudioAssetInstance() {
    return mAudioAssetInstance;
  }

  void SceneObject::setModelAssetInstance(AssetInstance* modelAsset) {
    mModelAssetInstance = modelAsset;
  }

  AssetInstance* SceneObject::getModelAssetInstance() {
    return mModelAssetInstance;
  }

  void SceneObject::setScriptAssetInstance(AssetInstance* scriptAsset) {
    mScriptAssetInstance = scriptAsset;
  }

  AssetInstance* SceneObject::getScriptAssetInstance() {
    return mScriptAssetInstance;
  }

  void SceneObject::setShaderAssetInstance(AssetInstance* shaderAsset) {
    mShaderAssetInstance = shaderAsset;
  }

  AssetInstance* SceneObject::getShaderAssetInstance() {
    return mShaderAssetInstance;
  }

  void SceneObject::setLightAssetInstance(AssetInstance* lightAsset) {
    mLightAssetInstance = lightAsset;
  }

  AssetInstance* SceneObject::getLightAssetInstance() {
    return mLightAssetInstance;
  }

  bool SceneObject::hasModelAssetInstance() {
    return mModelAssetInstance != nullptr;
  }

  bool SceneObject::hasShaderAssetInstance() {
    return mShaderAssetInstance != nullptr;
  }

  bool SceneObject::hasScriptAssetInstance() {
    return mScriptAssetInstance != nullptr;
  }


  void SceneObject::setPhysicsObjectAssetInstance(AssetInstance* physicsObject) {
    mPhysicsObjectAssetInstance = physicsObject;
  }

  AssetInstance* SceneObject::getPhysicsObjectAssetInstance() {
    return mPhysicsObjectAssetInstance;
  }

  std::string SceneObject::getTransformType() {
    return mTransformType;
  }

  void SceneObject::setTransformType(std::string transformType) {
    mTransformType = transformType;
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


} // End of Dream
