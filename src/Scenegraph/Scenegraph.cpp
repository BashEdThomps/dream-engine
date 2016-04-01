/*
* graph.c
*/

#include <iostream>
#include <string>
#include <sstream>
#include "Node.h"
#include "Scenegraph.h"

#define PATH_DELIMETER "/"

namespace DreamScenegraph {
	Scenegraph::Scenegraph(std::string name) {
		mName = name;

		for (int i=0;i<NODES;i++) {
			mNodes[i] = NULL;
		}

		mCamera = new Camera();
		mRootNode = NULL;
	}

	Node* Scenegraph::createNode() {
		int available = getNextAvailableNodeIndex();
		if (available < 0) {
			std::cout << "Error: Unable to create new node - tree is full" << std::endl;
			return NULL;
		}

		Node* retval = new Node();
		mNodes[available] = retval;
		return retval;
	}

	int Scenegraph::getNextAvailableNodeIndex() {
		int i;
		for (i=0; i<NODES; i++) {
			if (mNodes[i] == NULL) {
				return i;
			}
		}
		return -1;
	}

	int Scenegraph::getIndexOfNode(Node *node) {
		int i;
		for (i=0; i<NODES; i++) {
			if (mNodes[i] == node) {
				return i;
			}
		}
		return -1;
	}

	Node* Scenegraph::getRootNode() {
		return mRootNode;
	}

	void Scenegraph::setRootNode(Node* root) {
		mRootNode = root;
	}

	void Scenegraph::removeNode(Node* node) {
		if (node == NULL) {
			std::cout << "Cannot remove NULL node from graph." << std::endl;
			return;
		}

		int i = getIndexOfNode(node);
		if (i < 0) {
			std::cout <<"Cannot find node in graph to remove it." << std::endl;
			return;
		}

		delete node;
		mNodes[i] = NULL;
		return;
	}

	/*void Scenegraph::traverseNodeVector(void (*function)(Node*, void*), void* arg) {
		int i;
		Node* next;
		for (i=0;i<NODES;i++) {
			next = mNodes[i];
			if (next == NULL) {
				continue;
			}
			std::cerr << "This is not implemented :{" << std::endl;
			//(*function)((void*)next, arg == NULL ? this : arg);
		}
		return;
	}
    */

	Node* Scenegraph::getNodeByName(std::string name) {
		int i;
		Node* next;
		for (i=0;i<NODES;i++) {
			next = mNodes[i];
			if (next == NULL) {
				continue;
			}
			if (next->hasName("")) {
				std::cout << "Scenegraph::getNodeByName: Cannot test node with empty name." << std::endl;
				continue;
			}
			if (next->hasName(name)) {
				return mNodes[i];
			}
		}
		return NULL;
	}

	/*int Scenegraph::traversePath(std::string path, void (*function)(Node*, void*), void* arg) {
		std::string start    = NULL;
		std::string end      = NULL;
		int done       = 0;
		int nameLen    = 0;
		Node* target = NULL;
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
			target =Scenegraph::getNodeByName(graph,tmp);
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
		Node* next;
		for (i=0;i<NODES;i++) {
			next = mNodes[i];
			if (next != NULL) {
				generatePathForNode(next);
			}
		}
		return;
	}

	void Scenegraph::generatePathForNode(Node* node) {
		if (node == NULL) {
			std::cout <<"Cannot generate path for NULL node." << std::endl;;
			return;
		}

		if (node->getName().size() == 0) {
			std::cout <<"Cannot generate path for node with no name." << std::endl;;
			return;
		}

		std::stringstream pathBuffer;
		// Node has no parent
		if (node->getParent() == NULL) {
			// and is not graph root
			if (!isRootNode(node)) {
				std::cout << "Cannot generate path for non-root node with no parent." << std::endl;;
				return;
			}
			std::cout << "Generating path for node root " <<  node->getName() << std::endl;
			pathBuffer << PATH_DELIMETER;
			pathBuffer << node->getName();
		} else {
			Node* parent = node->getParent();
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

	bool Scenegraph::isRootNode(Node* node) {
		return mRootNode == node;
	}

	void Scenegraph::printGraph() {
		std::cout << "NOT IMPLEMENTED" << std::endl;
		return;
	}
}
