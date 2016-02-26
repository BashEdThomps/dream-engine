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

void dsgNodeInitOffset(node_t* node) {
	node->globalTraRot = (nodeTraRot_t*) malloc(sizeof(nodeTraRot_t));
	node->globalTraRot->transX = 0.0f;
	node->globalTraRot->transY = 0.0f;
	node->globalTraRot->transZ = 0.0f;
	node->globalTraRot->rotX = 0.0f;
	node->globalTraRot->rotY = 0.0f;
	node->globalTraRot->rotZ = 0.0f;

	node->localTraRot = (nodeTraRot_t*) malloc(sizeof(nodeTraRot_t));
	node->localTraRot->transX = 0.0f;
	node->localTraRot->transY = 0.0f;
	node->localTraRot->transZ = 0.0f;
	node->localTraRot->rotX = 0.0f;
	node->localTraRot->rotY = 0.0f;
	node->localTraRot->rotZ = 0.0f;
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

	if (parent == NULL || child == NULL || 
	    !dsgNodeHasValidPath(child) || !dsgNodeHasValidPath(parent) ||
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

void dsgNodeSumWithParentTraRot(node_t* node) {
	nodeTraRot_t* parent = node->parent;
	nodeTraRot_t* local  = node->localTraRot;

	node->globalTraRot->transX = parent->transX + local->transX;
	node->globalTraRot->transX = parent->transY + local->transY;
	node->globalTraRot->transX = parent->transZ + local->transZ;

	node->globalTransRot->rotX = parent->rotX + local->rotX;
	node->globalTransRot->rotY = parent->rotY + local->rotY;
	node->globalTransRot->rotZ = parent->rotZ + local->rotZ;
}
