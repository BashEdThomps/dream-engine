/*
* graph.c
*/

#include <iostream>
#include <string>
#include <sstream>
#include "SceneObject.h"
#include "Scenegraph.h"

#define PATH_DELIMETER "/"

namespace Dream {
	namespace Scene {
		namespace Scenegraph {
			Scenegraph::Scenegraph(std::string name) {
				mName = name;

				for (int i=0;i<NODES;i++) {
					mSceneObjects[i] = NULL;
				}

				mCamera = new Camera();
				mRootSceneObject = NULL;
			}

			SceneObject* Scenegraph::createSceneObject() {
				int available = getNextAvailableSceneObjectIndex();
				if (available < 0) {
					std::cout << "Error: Unable to create new Scene Object - tree is full" << std::endl;
					return NULL;
				}

				SceneObject* retval = new SceneObject();
				mSceneObjects[available] = retval;
				return retval;
			}

			int Scenegraph::getNextAvailableSceneObjectIndex() {
				int i;
				for (i=0; i<NODES; i++) {
					if (mSceneObjects[i] == NULL) {
						return i;
					}
				}
				return -1;
			}

			int Scenegraph::getIndexOfSceneObject(SceneObject *node) {
				int i;
				for (i=0; i<NODES; i++) {
					if (mSceneObjects[i] == node) {
						return i;
					}
				}
				return -1;
			}

			SceneObject* Scenegraph::getRootSceneObject() {
				return mRootSceneObject;
			}

			void Scenegraph::setRootSceneObject(SceneObject* root) {
				mRootSceneObject = root;
			}

			void Scenegraph::removeSceneObject(SceneObject* node) {
				if (node == NULL) {
					std::cout << "Cannot remove NULL node from graph." << std::endl;
					return;
				}

				int i = getIndexOfSceneObject(node);
				if (i < 0) {
					std::cout <<"Cannot find node in graph to remove it." << std::endl;
					return;
				}

				delete node;
				mSceneObjects[i] = NULL;
				return;
			}

			/*void Scenegraph::traverseSceneObjectVector(void (*function)(SceneObject*, void*), void* arg) {
				int i;
				SceneObject* next;
				for (i=0;i<NODES;i++) {
					next = mSceneObjects[i];
					if (next == NULL) {
						continue;
					}
					std::cerr << "This is not implemented :{" << std::endl;
					//(*function)((void*)next, arg == NULL ? this : arg);
				}
				return;
			}
		    */

			SceneObject* Scenegraph::getSceneObjectByName(std::string name) {
				int i;
				SceneObject* next;
				for (i=0;i<NODES;i++) {
					next = mSceneObjects[i];
					if (next == NULL) {
						continue;
					}
					if (next->hasName("")) {
						std::cout << "Scenegraph::getSceneObjectByName: Cannot test node with empty name." << std::endl;
						continue;
					}
					if (next->hasName(name)) {
						return mSceneObjects[i];
					}
				}
				return NULL;
			}

			/*int Scenegraph::traversePath(std::string path, void (*function)(SceneObject*, void*), void* arg) {
				std::string start    = NULL;
				std::string end      = NULL;
				int done       = 0;
				int nameLen    = 0;
				SceneObject* target = NULL;
				std::cout << "Traversing path: " << path << std::endl;
				start = path+1;
				if (start == NULL) {
					std::cout << "Path is null" << std::endl;
					return done;
				}
				while (!done) {
					nameLen = 0;
					end = strchr(start+1, Scenegraph::PathDelimeter);
					if (end == NULL) {
						std::cout <<"Found end of string" << std::endl;
						end = path+strlen(path);
						done = 1;
					}
					end += 1;
					nameLen = (end-start-1);
					if (nameLen <= 0) {
						std::cout <<"Traverse nameLen <= 0" << std::endl;;
						break;
					}
					 std::string tmp = ( std::string)malloc(sizeof(char)*nameLen+1);
					memcpy(tmp,start,nameLen);
					tmp[nameLen] = '\0';
					std::cout <<"Traversed path to: " << tmp << std::endl;
					target =Scenegraph::getSceneObjectByName(graph,tmp);
					if (target != NULL) {
						function(target,arg);
						start = end;
					} else {
						std::cout << tmp << " not found while traversing path" << std::endl;
						break;
					}
				}
				return done;
			}*/


			void Scenegraph::updatePaths() {
				int i;
				SceneObject* next;
				for (i=0;i<NODES;i++) {
					next = mSceneObjects[i];
					if (next != NULL) {
						generatePathForSceneObject(next);
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
