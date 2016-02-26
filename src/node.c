/*
* node.c
*/
#include <stdlib.h>
#include <string.h>
#include "node.h"

void dsgNodeInit(node_t* node, char* name) {
	node->name = name;
	node->parent = NULL;
	node->numChildren = 0;
	node->children = NULL;
	node->path = NULL;
}

void dsgNodeInitTransRot(node_t* node) {
	node->transRot = (nodeTransRot_t*)malloc(sizeof(nodeTransRot_t));
	node->transRot->transX = 0.0f;
	node->transRot->transY = 0.0f;
	node->transRot->transZ = 0.0f;
	node->transRot->rotX = 0.0f;
	node->transRot->rotY = 0.0f;
	node->transRot->rotZ = 0.0f;
}

int dsgNodeIsParentOf (node_t* parent, node_t* child) {
	int retval = 0; 

	if (!dsgNodeHasValidPath(child) || !dsgNodeHasValidPath(parent) ||
	    !dsgNodeHasValidName(child) || !dsgNodeHasValidName(parent)) {
		return retval;	
	}

	char* indexOfParent = strstr(child->path,parent->name); 
	char* indexOfChild = strstr(child->path,child->name);

	if (indexOfParent != NULL && indexOfChild != NULL) {
		retval = indexOfChild > indexOfParent;
	}

	return retval;
}

int dsgNodeIsChildOf(node_t* parent, node_t* child) {
	int retval = 0; 

	if (!dsgNodeHasValidPath(child) || !dsgNodeHasValidPath(parent) ||
	    !dsgNodeHasValidName(child) || !dsgNodeHasValidName(parent)) {
		return retval;	
	}
	char* indexOfParent = strstr(child->path,parent->name);
	char* indexOfChild = strstr(child->path,child->name);

	if (indexOfParent != NULL && indexOfChild != NULL) {
		retval = indexOfChild > indexOfParent;
	}

	return retval;
}

void dsgNodeSetParent(node_t* parent, node_t* child) {
	child->parent = parent;
}

int dsgNodeHasValidPath(node_t* node) {
	return node->path != NULL;
}

int dsgNodeHasValidName(node_t* node) {
	return node->name != NULL;
}
