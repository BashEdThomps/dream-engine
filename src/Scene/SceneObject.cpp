/*
* SceneObject.cpp
*/
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
			mParent = NULL;
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

		bool SceneObject::hasValidName() {
			return mName.size() > 0;
		}

		void SceneObject::setName(std::string name) {
			mName = name;
		}

		std::string SceneObject::getName() {
			return mName;
		}

		void SceneObject::showStatus() {
			std::cout << "SceneObject:"     << std::endl;
			std::cout << "          UUID: " << getUUID()       << std::endl;
			std::cout << "    ParentUUID: " << getParentUUID() << std::endl;
			std::cout << "          Name: " << getName()       << std::endl;
			std::cout << "          Path: " << generatePath()  << std::endl;
			return;
		}

		void SceneObject::setParent(SceneObject* parent) {
			mParent = parent;
			return;
		}

		SceneObject* SceneObject::getParent() {
			return mParent;
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

		int SceneObject::countChildren() {
			int retval = 0;
			std::cerr << "Count children is not implemented" << std::endl;
			return retval;
		}

		bool SceneObject::isChildOf(SceneObject* parent) {
			return mParent == parent;
		}

		bool SceneObject::isParentOf(SceneObject* child) {
			return child->getParent() == this;
		}

		void SceneObject::setParentUUID(std::string uuid) {
			mParentUUID = uuid;
		}

		std::string SceneObject::getParentUUID() {
			return mParentUUID;
		}

		void SceneObject::setUUID(std::string uuid) {
			mUUID = uuid;
		}

		std::string SceneObject::getUUID() {
			return mUUID;
		}


		std::string SceneObject::generatePath() {
			std::stringstream stream;
			std::vector<std::string> pathVector;
			SceneObject* next = this;
			while (next != NULL) {
				pathVector.push_back(next->getUUID());
				next = next->getParent();
			}
			std::reverse(pathVector.begin(),pathVector.end());
			for (std::vector<std::string>::iterator it = pathVector.begin();
			     it != pathVector.end(); ++it) {
					 stream << PATH_DELIMETER << *it;
			}
			std::string retval = stream.str();
			return retval;
		}
	}
}
