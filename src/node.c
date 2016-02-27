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
	node->numImmediateChildren = 0;
	node->immediateChildren = NULL;
	node->path = NULL;
}

void dsgNodeInitTranslationRotation(node_t* node) {
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

int dsgNodeIsImmediateChildOf(node_t* parent, node_t* child) {
	int retval = 0; 

	if (parent == NULL || child == NULL || 
	    !dsgNodeHasValidPath(child) || !dsgNodeHasValidPath(parent) ||
	    !dsgNodeHasValidName(child) || !dsgNodeHasValidName(parent)) {
		return retval;	
	}
	retval = child->parent == (node_t*)parent;
	return retval;
}
void dsgNodeSetParent(node_t* parent, node_t* child) {
	child->parent = (struct _node_t*)parent;
}

int dsgNodeHasValidPath(node_t* node) {
	return node->path != NULL;
}

int dsgNodeHasValidName(node_t* node) {
	return node->name != NULL;
}

void dsgNodeSumWithParentTranslationRotation(node_t* node) {
	nodeTraRot_t* local  = node->localTraRot;

	if (node->parent == NULL) {
		node_t* parentNode = (node_t*)node->parent;
		nodeTraRot_t* parent = parentNode->globalTraRot;	
		node->globalTraRot->transX = parent->transX + local->transX;
		node->globalTraRot->transX = parent->transY + local->transY;
		node->globalTraRot->transX = parent->transZ + local->transZ;
		node->globalTraRot->rotX = parent->rotX + local->rotX;
		node->globalTraRot->rotY = parent->rotY + local->rotY;
		node->globalTraRot->rotZ = parent->rotZ + local->rotZ;

	} else {
		node->globalTraRot->transX = local->transX;
		node->globalTraRot->transX = local->transY;
		node->globalTraRot->transX = local->transZ;
		node->globalTraRot->rotX = local->rotX;
		node->globalTraRot->rotY = local->rotY;
		node->globalTraRot->rotZ = local->rotZ;
	}
	return;
}
