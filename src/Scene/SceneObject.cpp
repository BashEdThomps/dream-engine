#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <algorithm>
#include "SceneObject.h"
#include "../Util/String.h"

namespace Dream {
namespace Scene {
	
	SceneObject::SceneObject() {}

	SceneObject::SceneObject(nlohmann::json soJson){
		mTranslation = std::vector<float>(3);
		mRotation = std::vector<float>(3);
		mScale = std::vector<float>(3);
		
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
		
		mAnimationAssetInstance = NULL;
		mAudioAssetInstance     = NULL;
		mModelAssetInstance     = NULL;
		mScriptAssetInstance    = NULL;
		
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
		mTranslation = translation;
#ifdef VERBOSE
		std::cout << "SceneObject: " << getNameUUIDString() << " translation " << Util::String::floatVectorToString(mTranslation) << std::endl;
#endif
	}
	
	void SceneObject::setRotation(std::vector<float> rotation) {
		mRotation = rotation;
#ifdef VERBOSE
		std::cout << "SceneObject: " << getNameUUIDString() << " rotation " << Util::String::floatVectorToString(mRotation) << std::endl;
#endif
	}
	
	void SceneObject::setScale(std::vector<float> scale) {
		mScale = scale;
#ifdef VERBOSE
		std::cout << "SceneObject: " << getNameUUIDString() << " scale " << Util::String::floatVectorToString(mScale) << std::endl;
#endif
	}

	void SceneObject::setTranslation(float x, float y, float z) {
		mTranslation[SO_X] = x;
		mTranslation[SO_Y] = y;
		mTranslation[SO_Z] = z;
#ifdef VERBOSE
		std::cout << "SceneObject: " << getNameUUIDString() << " translation " << Util::String::floatVectorToString(mTranslation) << std::endl;
#endif
	}

	void SceneObject::setRotation(float x, float y, float z) {
		mRotation[SO_X] = x;
		mRotation[SO_Y] = y;
		mRotation[SO_Z] = z;
#ifdef VERBOSE
		std::cout << "SceneObject: " << getNameUUIDString() << " rotation " << Util::String::floatVectorToString(mRotation) << std::endl;
#endif
	}
	
	void SceneObject::setScale(float x, float y, float z) {
		mScale[SO_X] = x;
		mScale[SO_Y] = y;
		mScale[SO_Z] = z;
#ifdef VERBOSE
		std::cout << "SceneObject: " << getNameUUIDString() << " scale " << Util::String::floatVectorToString(mScale) << std::endl;
#endif
	}

	std::vector<float> SceneObject::getRotation() {
		return mRotation;
	}
	
	std::vector<float> SceneObject::getScale() {
		return mScale;
	}
	
	std::vector<float> SceneObject::getTranslation() {
		return mTranslation;
	}
	
	void SceneObject::setAssetInstanceParentToThis(Asset::AssetInstance* asset) {
		asset->setParentSceneObject(this);
	}
	
	void SceneObject::setParent(SceneObject* parent) {
		mParent = parent;
	}

	SceneObject* SceneObject::getParent() {
		return mParent;
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

	int  SceneObject::countChildren() {
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

	bool SceneObject::isParentOf(SceneObject*) {
		return false;
	}

	bool SceneObject::isParentOfDeep(SceneObject*) {
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
		while (current != NULL) {
			mPath = current->getUUID()+mPath;
			current = current->getParent();
			if (current != NULL) {
				mPath = PATH_DELIMETER+mPath;
			}
		}
	}

	std::string SceneObject::getPath() {
		return mPath;
	}

	void SceneObject::showStatus() {
		std::cout << "SceneObject:" << std::endl;
		std::cout << "         UUID: " << mUUID << std::endl;
		std::cout << "         Name: " << mName << std::endl;
		std::cout << "   ParentUUID: ";
		
		SceneObject* parent = getParent();
		if (parent != NULL) {
			 std::cout << getParent()->getUUID();
		} else {
			std::cout << "None";
		}
		std::cout << std::endl;
		std::cout << "      Children: " << mChildren.size() << std::endl;
		std::cout << "Trnasform Type: " << mTransformType << std::endl;
		std::cout << "   Translation: " << Util::String::floatVectorToString(mTranslation) << std::endl;
		std::cout << "      Rotation: " << Util::String::floatVectorToString(mRotation)<< std::endl;
		std::cout << "         Scale: " << Util::String::floatVectorToString(mRotation)<< std::endl;
	}
	
	std::vector<std::string> SceneObject::getAssetInstanceUUIDsToLoad() {
		return mAssetInstanceUUIDsToLoad;
	}
	
	void SceneObject::setAnimationAssetInstance(Asset::AssetInstance* animationAsset) {
		mAnimationAssetInstance = animationAsset;
		setAssetInstanceParentToThis(animationAsset);
	}
	
	Asset::AssetInstance* SceneObject::getAnimationAssetInstance() {
		return mAnimationAssetInstance;
	}
	
	void SceneObject::setAudioAssetInstance(Asset::AssetInstance* audioAsset) {
		mAudioAssetInstance = audioAsset;
		setAssetInstanceParentToThis(audioAsset);
	}
	
	Asset::AssetInstance* SceneObject::getAudioAssetInstance() {
		return mAudioAssetInstance;
	}
	
	void SceneObject::setModelAssetInstance(Asset::AssetInstance* modelAsset) {
		mModelAssetInstance = modelAsset;
		setAssetInstanceParentToThis(modelAsset);
	}
	
	Asset::AssetInstance* SceneObject::getModelAssetInstance() {
		return mModelAssetInstance;
	}
	
	void SceneObject::setScriptAssetInstance(Asset::AssetInstance* scriptAsset) {
		mScriptAssetInstance = scriptAsset;
		setAssetInstanceParentToThis(scriptAsset);
	}
	
	Asset::AssetInstance* SceneObject::getScriptAssetInstance() {
		return mScriptAssetInstance;
	}
	
	void SceneObject::setShaderAssetInstance(Asset::AssetInstance* shaderAsset) {
		mShaderAssetInstance = shaderAsset;
		setAssetInstanceParentToThis(shaderAsset);
	}
	
	Asset::AssetInstance* SceneObject::getShaderAssetInstance() {
		return mShaderAssetInstance;
	}
	
	void SceneObject::setLightAssetInstance(Asset::AssetInstance* lightAsset) {
		mLightAssetInstance = lightAsset;
		setAssetInstanceParentToThis(lightAsset);
	}
	
	Asset::AssetInstance* SceneObject::getLightAssetInstance() {
		return mLightAssetInstance;
	}

	bool SceneObject::hasModelAssetInstance() {
		return mModelAssetInstance != NULL;
	}
	
	bool SceneObject::hasShaderAssetInstance() {
		return mShaderAssetInstance != NULL;
	}
	
	bool SceneObject::hasScriptAssetInstance() {
		return mScriptAssetInstance != NULL;
	}


	void SceneObject::setPhysicsObjectAssetInstance(Asset::AssetInstance* physicsObject) {
		mPhysicsObjectAssetInstance = physicsObject;
	}
	
	Asset::AssetInstance* SceneObject::getPhysicsObjectAssetInstance() {
		return mPhysicsObjectAssetInstance;
	}
	
	std::string SceneObject::getTransformType() {
		return mTransformType;
	}
	
	void SceneObject::setTransformType(std::string transformType) {
		mTransformType = transformType;
	}
	
} // End of Scene
} // End of Dream
