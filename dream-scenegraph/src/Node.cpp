/*
* Node.cpp
*/
#include <string>
#include <math>
#include "Node.h"

namespace DreamScenegraph {
	Node(char* name) {
		mName = name;
		mParentIndex = -1;
		mPath = NULL;
		mVertexBufferIndex = -1;
		initTranslationRotation(node);
	}

	~Node() {

	}
	void Node::initTranslationRotation() {
		mTranslation[NODE_X] = 0.0f;
		mTranslation[NODE_Y] = 0.0f;
		mTranslation[NODE_Z] = 0.0f;

		mRotation[NODE_X] =  0.0f;
		mRotation[NODE_Y] =  0.0f;
		mRotation[NODE_Z] =  0.0f;
		return;
	}

	int Node::HasValidPath() {
		return mPath != NULL;
	}

	int Node::HasValidName() {
		return mName != NULL;
	}

	void Node::print() {
		std::cout << mPath << std::endl;
		return;
	}

	void Node::setParentIndex(int parent) {
		mParentIndex = parent;
		return;
	}

	void Node::setTranslation(float x, float y, float z) {
		mTranslation[NODE_X] = x;
		mTranslation[NODE_Y] = y;
		mTranslation[NODE_Z] = z;
		return;
	}

	void Node::setRotation(float x, float y, float z) {
		mRotation[NODE_X] = x;
		mRotation[NODE_Y] = y;
		mRotation[NODE_Z] = z;
		return;
	}

	int Node::hasVertexBuffer() {
		return mVertexBufferIndex > -1;
	}

	void Node::nodeSetPolarTranslation(float radius, float theta) {
		return;
	}
}
