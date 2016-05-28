#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <algorithm>
#include "SceneObject.h"

namespace Dream {
namespace Scene {
	
	SceneObject::SceneObject() {}

	SceneObject::SceneObject(nlohmann::json soJson){
		if(!soJson[SCENE_OBJECT_UUID].is_null()){
			mUUID = soJson[SCENE_OBJECT_UUID];
		}

		if (!soJson[SCENE_OBJECT_NAME].is_null()) {
			mName = soJson[SCENE_OBJECT_NAME];
		}
		
		if (!soJson[SCENE_OBJECT_POSITION_TYPE].is_null()) {
			mPositionType = soJson[SCENE_OBJECT_POSITION_TYPE];
		} else {
			mPositionType = SCENE_OBJECT_POSITION_TYPE_OFFSET;
		}
		
		if (!soJson[SCENE_OBJECT_TRANSLATION].is_null()) {
			nlohmann::json translation = soJson[SCENE_OBJECT_TRANSLATION];
			mTranslation[NODE_X] = translation[SCENE_OBJECT_X];
			mTranslation[NODE_Y] = translation[SCENE_OBJECT_Y];
			mTranslation[NODE_Z] = translation[SCENE_OBJECT_Z];
		} else {
			mTranslation[NODE_X] = 0.0f;
			mTranslation[NODE_Y] = 0.0f;
			mTranslation[NODE_Z] = 0.0f;
		}
			
		if (!soJson[SCENE_OBJECT_ROTATION].is_null()) {
			nlohmann::json rotation = soJson[SCENE_OBJECT_ROTATION];
			mRotation[NODE_X] = rotation[SCENE_OBJECT_X];
			mRotation[NODE_Y] = rotation[SCENE_OBJECT_Y];
			mRotation[NODE_Z] = rotation[SCENE_OBJECT_Z];
		} else {
			mRotation[NODE_X] = 0.0f;
			mRotation[NODE_Y] = 0.0f;
			mRotation[NODE_Z] = 0.0f;
		}
		
		if (!soJson[SCENE_OBJECT_SCALE].is_null()) {
			nlohmann::json scale = soJson[SCENE_OBJECT_SCALE];
			mScale[NODE_X] = scale[SCENE_OBJECT_X];
			mScale[NODE_Y] = scale[SCENE_OBJECT_Y];
			mScale[NODE_Z] = scale[SCENE_OBJECT_Z];
		} else {
			mScale[NODE_X] = 0.0f;
			mScale[NODE_Y] = 0.0f;
			mScale[NODE_Z] = 0.0f;
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
	
	void SceneObject::resetTranslationRotation() {
		mTranslation [NODE_X] = 0.0f;
		mTranslation [NODE_Y] = 0.0f;
		mTranslation [NODE_Z] = 0.0f;
		mRotation    [NODE_X] =  0.0f;
		mRotation    [NODE_Y] =  0.0f;
		mRotation    [NODE_Z] =  0.0f;
		return;
	}

	SceneObject::~SceneObject() {

	}

	bool SceneObject::hasName(std::string name) {
		return mName == name;
	}

	void SceneObject::setName(std::string name) {
		mName = name;
	}

	std::string SceneObject::getName() {
		return mName;
	}

	void SceneObject::setTranslation(float x, float y, float z) {
		mTranslation[NODE_X] = x;
		mTranslation[NODE_Y] = y;
		mTranslation[NODE_Z] = z;
		return;
	}

	float* SceneObject::getTranslation() {
		return mTranslation;
	}

	void SceneObject::setRotation(float x, float y, float z) {
		mRotation[NODE_X] = x;
		mRotation[NODE_Y] = y;
		mRotation[NODE_Z] = z;
		return;
	}

	float* SceneObject::getRotation() {
		return mRotation;
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

	void SceneObject::removeChild(SceneObject* child) {

	}

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
		std::cout << "     Children: " << mChildren.size() << std::endl;
		
		std::cout << "Position Type: " << mPositionType << std::endl;
		std::cout << "  Translation: (" << mTranslation[NODE_X] << ","
		                                << mTranslation[NODE_Y] << ","
		                                << mTranslation[NODE_Z] << ")"
		                                << std::endl;
		std::cout << "     Rotation: (" << mRotation[NODE_X] << ","
		                                << mRotation[NODE_Y] << ","
		                                << mRotation[NODE_Z] << ")"
		                                << std::endl;
	}
	
	std::vector<std::string> SceneObject::getAssetInstanceUUIDsToLoad() {
		return mAssetInstanceUUIDsToLoad;
	}
	
	void SceneObject::setAnimationAssetInstance(Dream::Asset::AssetInstance* animationAsset) {
		mAnimationAssetInstance = animationAsset;
	}
	
	Dream::Asset::AssetInstance* SceneObject::getAnimationAssetInstance() {
		return mAnimationAssetInstance;
	}
	
	void SceneObject::setAudioAssetInstance(Dream::Asset::AssetInstance* audioAsset) {
		mAudioAssetInstance = audioAsset;
	}
	Dream::Asset::AssetInstance* SceneObject::getAudioAssetInstance() {
		return mAudioAssetInstance;
	}
	
	void SceneObject::setModelAssetInstance(Dream::Asset::AssetInstance* modelAsset) {
		mModelAssetInstance = modelAsset;
	}
	
	Dream::Asset::AssetInstance* SceneObject::getModelAssetInstance() {
		return mModelAssetInstance;
	}
	
	void SceneObject::setScriptAssetInstance(Dream::Asset::AssetInstance* scriptAsset) {
		mScriptAssetInstance = scriptAsset;
	}
	
	Dream::Asset::AssetInstance* SceneObject::getScriptAssetInstance() {
		return mScriptAssetInstance;
	}
	
	void SceneObject::setShaderAssetInstance(Dream::Asset::AssetInstance* shaderAsset) {
		mShaderAssetInstance = shaderAsset;
	}
	
	Dream::Asset::AssetInstance* SceneObject::getShaderAssetInstance() {
		return mShaderAssetInstance;
	}
	
	bool SceneObject::hasModelAssetInstance() {
		return mModelAssetInstance != NULL;
	}
	
	bool SceneObject::hasShaderAssetInstance() {
		return mShaderAssetInstance != NULL;
	}
		
} // End of Scene
} // End of Dream
