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
			mRootSceneObject = NULL;
		}

		Scenegraph::~Scenegraph() {

		}

		SceneObject* Scenegraph::getRootSceneObject() {
			return mRootSceneObject;
		}

		bool Scenegraph::hasSceneObect(SceneObject *obj) {
			return std::find(mSceneObjects.begin(), mSceneObjects.end(), obj) != mSceneObjects.end();
		}

		void Scenegraph::setRootSceneObject(SceneObject* root) {
			if (!hasSceneObect(root)) {
				mSceneObjects.push_back(root);
			}
			mRootSceneObject = root;
			mRootSceneObject->generatePath();
		}

		void Scenegraph::addSceneObjectTo(SceneObject* child, SceneObject* parent) {
			child->setParent(parent);
			child->generatePath();

			mSceneObjects.push_back(child);
		}

		void Scenegraph::removeSceneObject(SceneObject* so) {
			if (so == NULL) {
				#ifdef DEBUG
				std::cout << "Cannot remove NULL node from graph." << std::endl;
				#endif
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
					#ifdef DEBUG
					std::cout << "Scenegraph::getSceneObjectByName: Cannot test node with empty name." << std::endl;
					#endif
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
			int count = 0;
			for (std::vector<SceneObject*>::iterator next = mSceneObjects.begin() ;
			     next != mSceneObjects.end();
				 ++next) {
					 if ((*next)->getParent() == obj) count++;
				 }
				 return count;
		}
	}
}
