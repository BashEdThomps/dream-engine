#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <algorithm>
#include "SceneObject.h"

namespace Dream {
	namespace Scene {
		SceneObject::SceneObject() {
			init();
		}

		SceneObject::SceneObject(nlohmann::json soJson){
			init();
			if(!soJson[SCENE_OBJECT_JSON_UUID].is_null()){
				mUUID = soJson[SCENE_OBJECT_JSON_UUID];
			}

			if (!soJson[SCENE_OBJECT_JSON_NAME].is_null()) {
				mName = soJson[SCENE_OBJECT_JSON_NAME];
			}
		}

		void SceneObject::init() {
			initTranslationRotation();
		}

		void SceneObject::initTranslationRotation() {
			mTranslation[NODE_X] = 0.0f;
			mTranslation[NODE_Y] = 0.0f;
			mTranslation[NODE_Z] = 0.0f;
			mRotation[NODE_X] =  0.0f;
			mRotation[NODE_Y] =  0.0f;
			mRotation[NODE_Z] =  0.0f;
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

		void SceneObject::showStatus() {
			std::cout << "SceneObject:"     << std::endl;
			std::cout << "          UUID: " << getUUID() << std::endl;
			std::cout << "    ParentUUID: ";
			SceneObject* parent = getParent();
			if (parent != NULL) {
				 std::cout << getParent()->getUUID();
			} else {
				std::cout << "None";
			}
			std::cout << std::endl;

			std::cout << "          Name: " << getName() << std::endl;
			//std::cout << "          Path: " << generatePath()  << std::endl;
			return;
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
			for (
				std::vector<SceneObject*>::iterator it = mChildren.begin();
				it != mChildren.end();
				it++) {
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
	}
}
