/*
* node.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "node.h"

void dsgNodeInit(node_t* node, char* name) {
	node->name = name;
	node->parentIndex = -1;
	node->numChildren =  0;
	node->children    = NULL;
	node->numImmediateChildren = 0;
	node->immediateChildren    = NULL;
	node->path = NULL;
	dsgNodeInitTranslationRotation(node);
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

int dsgNodeHasValidPath(node_t* node) {
	if (node == NULL) return 0;
	return node->path != NULL;
}

int dsgNodeHasValidName(node_t* node) {
	if (node == NULL) return 0;
	return node->name != NULL;
}

void dsgNodeSumWithParentTranslationRotation(void* arg) {
	if (arg == NULL) {
		return;	
	}

	dsgNodeSumWithParentArg_t* castArg = (dsgNodeSumWithParentArg_t*)arg;

	node_t* node = castArg->node;
	node_t* parent = castArg->parent;

	if (node == NULL) {
		fprintf(stderr,"Node is NULL in transform sum\n");	
		return;
	}

	nodeTraRot_t* localTr  = node->localTraRot;
	nodeTraRot_t* globalTr = node->globalTraRot;
	if (parent == NULL) {
		fprintf(stderr,"Parent is NULL in transform sum\n");
		globalTr->transX  = localTr->transX;
		globalTr->transX  = localTr->transY;
		globalTr->transX  = localTr->transZ;
		globalTr->rotX    = localTr->rotX;
		globalTr->rotY    = localTr->rotY;
		globalTr->rotZ    = localTr->rotZ;
		return;
	}
	else {
		nodeTraRot_t* parentTr = parent->globalTraRot;

		if (parentTr == NULL) {
			fprintf(stderr,"The parent global transformation object is null\n");
			return;	
		}

		if (localTr == NULL) {
			fprintf(stderr,"The local global transformation object is null\n");
			return;	
		}

		globalTr->transX = parentTr->transX + localTr->transX;
		globalTr->transX = parentTr->transY + localTr->transY;
		globalTr->transX = parentTr->transZ + localTr->transZ;
		globalTr->rotX   = parentTr->rotX   + localTr->rotX;
		globalTr->rotY   = parentTr->rotY   + localTr->rotY;
		globalTr->rotZ   = parentTr->rotZ   + localTr->rotZ;
	}

	return;
}

void dsgNodePrint(node_t* node, void* arg) {
	fprintf(stdout,"%s\n", node->path);
	return;
}

void dsgNodeSetParentIndex(int parent, node_t* node) {
	node->parentIndex = parent;
	return;
}
