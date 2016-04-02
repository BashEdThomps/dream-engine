/*
* SceneObject.cpp
*/
#include <iostream>
#include <cmath>
#include "SceneObject.h"

namespace Dream {
	namespace Scene {
		namespace Scenegraph {
			SceneObject::SceneObject(std::string name) {
				mName = name;
				init();
			}

			SceneObject::SceneObject() {
				init();
			}

			void SceneObject::init() {
				mParent = NULL;

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

			int SceneObject::hasValidPath() {
				return mPath.size() > 0;
			}

			bool SceneObject::hasName(std::string name) {
				return mName == name;
			}

			int SceneObject::hasValidName() {
				return mName.size() > 0;
			}

			std::string SceneObject::getName() {
				return mName;
			}

			void SceneObject::print() {
				std::cout << mPath << std::endl;
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

			void SceneObject::setRotation(float x, float y, float z) {
				mRotation[NODE_X] = x;
				mRotation[NODE_Y] = y;
				mRotation[NODE_Z] = z;
				return;
			}

			void SceneObject::setPolarTranslation(float radius, float theta) {
				return;
			}

			int SceneObject::countChildren() {
				int retval = 0;
				std::cerr << "Count children is not implemented" << std::endl;
				return retval;
			}


			std::string SceneObject::getPath() {
				return mPath;
			}
			void SceneObject::setPath(std::string path) {
				mPath = path;
			}
		}
	}
}
