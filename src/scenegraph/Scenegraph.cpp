/*
* graph.c
*/

#include <iostream>
#include <string>
#include "Node.h"
#include "Scenegraph.h"

#define PATH_DELIMETER '/'

namespace DreamScenegraph {

	Scenegraph::Scenegraph(std::string name = NULL) {
		mName = name;

		int i;
		for (i=0;i<NODES;i++) {
			mNodes[i] = NULL;
		}

		mCamera = new Camera();
		mRootNodeIndex = -1;
	}

	Node* Scenegraph::createNode() {
		int available = getNextAvailableNode();
		if (available < 0) {
			std::cout << "Error: Unable to create new node - tree is full" << std::endl;
			return NULL;
		}
		Node* retval = new Node();
		mNodes[available] = retval;
		return retval;
	}

	int Scenegraph::getNextAvailableNode() {
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
		return mNodes[mRootNodeIndex];
	}

	void Scenegraph::setRootNode(Node* root) {
		mRootNodeIndex = getIndexOfNode(root);
	}

	void Scenegraph::setRootNodeIndex(int root) {
		mRootNodeIndex = root;
		return;
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

	void Scenegraph::traverseNodeVector(void (*function)(Node*, void*), void* arg) {
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

	int Scenegraph::traversePath(std::string path, void (*function)(Node*, void*), void* arg) {
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
	}


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
		if (node->getParentIndex() == -1) {
			// and is not graph root
			if (!isRootNode(node)) {
				std::cout << "Cannot generate path for non-root node with no parent." << std::endl;;
				return;
			}
			std::cout << "Generating path for node root " <<  node->getName() << std::endl;
			pathBuffer << &pathDelimeter;
			pathBuffer << node->getName();
		} else {
			Node* parent = mNodes[node->getParentIndex()];
			if (parent == NULL) {
				std::cout <<"Error finding node parent to create path" << std::endl;;
				return;
			}
			std::cout  <<  "Generating path for node " << node->getName() << " with parent " <<  parent->name << std::endl;
			pathBuffer <<  parent->getPath();
			pathBuffer << &pathDelimeter;
			pathBuffer <<  node->name;
		}

		node->setPath(pathBuffer.str());
		std::cout << "Generated path: " << node->getPath() << std::endl;
		return;
	}

	int Scenegraph::isRootNode(Node* node) {
		return mRootNodeIndex == getIndexOfNode(node);
	}

	int Scenegraph::addVertexBuffer(void* buffer) {
		std::cout <<"Adding vertex buffer to graph" << std::endl;
		int index = Scenegraph::getNextAvailableVertexBuffer();
		if (index < 0) {
			std::cout <<"Unable to add vertex buffer, graph is full" << std::endl;
			return -1;
		}
		mVertexBuffers[index] = (float*)buffer;
		return index;
	}

	int Scenegraph::getNextAvailableVertexBuffer() {
		int retval = -1;
		int i;
		for (i=0; i<VERT_BUFFERS; i++) {
			if (mVertexBuffers[i] == NULL) {
				retval = i;
				break;
			}
		}
		return retval;
	}

	int Scenegraph::getIndexOfVertexBuffer(void* buffer) {
		if(buffer == NULL) {
			return -1;
		}

		int retval = -1;
		int i;
		for (i=0; i<VERT_BUFFERS; i++) {
			if (mVertexBuffers[i] == buffer) {
				retval = i;
				break;
			}
		}
		return retval;
	}

	int Scenegraph::removeVertexBuffer(void* buffer) {
		if (buffer == NULL) {
			return -1;
		}

		int index = Scenegraph::getIndexOfNode(buffer);
		if (index < 0) {
			std::cerr << "Cannot remove vertex buffer from graph "
			          << mName != NULL ? graph->name : "Unamed Graph"
				  << ", not found"
				  << std::endl;
			return -1;
		}

		delete mVertexBuffers[index];
		mVertexBuffers[index] = NULL;
		return index;
	}

	int Scenegraph::nodeIsParentOf (Node* parent, Node* child) {
		int retval = 0;
		if (parent == NULL || !child->hasValidName() || !child->hasValidPath()) {
			return retval;
		}
		retval = child->getParentIndex() == getIndexOfNode(parent);
		return retval;
	}

	int Scenegraph::nodeIsChildOf(Node* parent, Node* child) {
		if (parent == NULL || child == NULL ||
		    child->hasValidPath() || child->hasValidName()) {
			return 0;
		}
		return child->getParentNodeIndex() == getIndexOfNode(parent);
	}

	void Scenegraph::setParentNode(Node* parent, Node* node) {
		node->setParentIndex(getIndexOfNode(parent),node);
		return;
	}

	void Scenegraph::printGraph() {
		std::cout << "NOT IMPLEMENTED" << std::endl;
		return;
	}

}
