/*
* graph.c
*/

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include "SceneObject.h"
#include "Scenegraph.h"

namespace Dream {
	namespace Scene {

		Scenegraph::Scenegraph() {
			mPathDelimeter = "/";
			mRootSceneObject = NULL;
		}

		Scenegraph::~Scenegraph() {

		}

		SceneObject* Scenegraph::getRootSceneObject() {
			return mRootSceneObject;
		}

		void Scenegraph::setRootSceneObject(SceneObject* root) {
			mRootSceneObject = root;
		}

		void Scenegraph::addSceneObjectTo(SceneObject* child, SceneObject* parent) {
			child->setParent(parent);
			child->generatePath();
			mSceneObjects.push_back(child);
		}

		void Scenegraph::removeSceneObject(SceneObject* so) {
			if (so == NULL) {
				std::cout << "Cannot remove NULL node from graph." << std::endl;
				return;
			}

			mSceneObjects.erase(std::remove(mSceneObjects.begin(), mSceneObjects.end(), so), mSceneObjects.end());
			delete so;
			return;
		}

		SceneObject* Scenegraph::getSceneObjectByName(std::string name) {
			for (std::vector<SceneObject*>::iterator next = mSceneObjects.begin() ;
			     next != mSceneObjects.end();
				 ++next) {
				if ((*next)->hasName("")) {
					std::cout << "Scenegraph::getSceneObjectByName: Cannot test node with empty name." << std::endl;
					continue;
				}
				if ((*next)->hasName(name)) {
					return (*next);
				}
			}
			return NULL;
		}

		bool Scenegraph::isRootSceneObject(SceneObject* node) {
			return mRootSceneObject == node;
		}

		void Scenegraph::printGraph() {
			std::cout << "NOT IMPLEMENTED" << std::endl;
			return;
		}


		int Scenegraph::getNumberOfSceneObjects() {
			return mSceneObjects.size();
		}

		int Scenegraph::countChildrenOfSceneObject(SceneObject* obj) {
			return -1;
		}
	}
}
