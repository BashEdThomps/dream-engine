/*
* graph.c
*/

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include "SceneObject.h"
#include "Scenegraph.h"

#define PATH_DELIMETER "/"

namespace Dream {
	namespace Scene {
		namespace Scenegraph {
			Scenegraph::Scenegraph() {
				mCamera = new Camera();
				mRootSceneObject = NULL;
			}

			Scenegraph::~Scenegraph() {

			}

			SceneObject* Scenegraph::createSceneObject() {
				SceneObject* retval = new SceneObject();
				mSceneObjects.push_back(retval);
				return retval;
			}

			SceneObject* Scenegraph::getRootSceneObject() {
				return mRootSceneObject;
			}

			void Scenegraph::setRootSceneObject(SceneObject* root) {
				mRootSceneObject = root;
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

			void Scenegraph::updatePaths() {
				for (std::vector<SceneObject*>::iterator next = mSceneObjects.begin() ;
				     next != mSceneObjects.end();
					 ++next) {
					if (*next != NULL) {
						generatePathForSceneObject(*next);
					}
				}
				return;
			}

			void Scenegraph::generatePathForSceneObject(SceneObject* node) {
				if (node == NULL) {
					std::cout <<"Cannot generate path for NULL node." << std::endl;;
					return;
				}

				if (node->getName().size() == 0) {
					std::cout <<"Cannot generate path for node with no name." << std::endl;;
					return;
				}

				std::stringstream pathBuffer;
				// SceneObject has no parent
				if (node->getParent() == NULL) {
					// and is not graph root
					if (!isRootSceneObject(node)) {
						std::cout << "Cannot generate path for non-root node with no parent." << std::endl;;
						return;
					}
					std::cout << "Generating path for node root " <<  node->getName() << std::endl;
					pathBuffer << PATH_DELIMETER;
					pathBuffer << node->getName();
				} else {
					SceneObject* parent = node->getParent();
					if (parent == NULL) {
						std::cout <<"Error finding node parent to create path" << std::endl;;
						return;
					}
					std::cout  <<  "Generating path for node " << node->getName() << " with parent " <<  parent->getName() << std::endl;
					pathBuffer <<  parent->getPath() << PATH_DELIMETER <<  node->getName();
				}

				node->setPath(pathBuffer.str());
				std::cout << "Generated path: " << node->getPath() << std::endl;
				return;
			}

			bool Scenegraph::isRootSceneObject(SceneObject* node) {
				return mRootSceneObject == node;
			}

			void Scenegraph::printGraph() {
				std::cout << "NOT IMPLEMENTED" << std::endl;
				return;
			}
		}
	}
}
