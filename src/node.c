/*
* node.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "node.h"

#define M_PI 3.14159265358979323846

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
void dsgNodeDestroy(node_t* node) {
	if (node->name != NULL)     free(node->name);
	if (node->children != NULL) free(node->children);
	if (node->path != NULL)     free(node->path);
	if (node->globalTraRot != NULL) free(node->globalTraRot);
	if (node->localTraRot != NULL)  free(node->localTraRot);
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

float toDegrees(float radians) {
    return radians * (180.0 / M_PI);
}

float toRadians(float degrees) {
	return (M_PI / 180.0f) * degrees;
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
		globalTr->transY  = localTr->transY;
		globalTr->transZ  = localTr->transZ;
		globalTr->rotX    = localTr->rotX;
		globalTr->rotY    = localTr->rotY;
		globalTr->rotZ    = localTr->rotZ;
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

		//globalTr->transX = parentTr->transX + localTr->transX;
		//globalTr->transY = parentTr->transY + localTr->transY;
		//globalTr->transZ = parentTr->transZ + localTr->transZ;

		globalTr->rotX   = parentTr->rotX + localTr->rotX;
		globalTr->rotY   = parentTr->rotY + localTr->rotY;
		globalTr->rotZ   = parentTr->rotZ + localTr->rotZ;

		float radius = localTr->transX;
		globalTr->transX = parentTr->transX + toRadians(radius) * cos(globalTr->rotY);
		globalTr->transZ = parentTr->transZ + toRadians(radius) * sin(globalTr->rotY);

		
	}
	fprintf(stdout,"%s T(%.2f,%.2f,%.2f) R(%.2f,%.2f,%.2f)\n",  node->name,
			globalTr->transX, globalTr->transY, globalTr->transZ,
			globalTr->rotX, globalTr->rotY, globalTr->rotZ);
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
