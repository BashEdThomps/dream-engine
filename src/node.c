/*
* node.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "node.h"

void dsgNodeInit(node_t* node, char* name) {
	node->name = name;
	node->parentIndex = -1;
	node->path = NULL;
	node->vertexBufferIndex = -1;
	dsgNodeInitTranslationRotation(node);
}
void dsgNodeDestroy(node_t* node) {
	if (node->name != NULL)        free(node->name);
	if (node->path != NULL)        free(node->path);
	if (node->translation != NULL) free(node->translation);
	if (node->rotation != NULL)    free(node->rotation);
}
void dsgNodeInitTranslationRotation(node_t* node) {
	node->translation = (float*)malloc(sizeof(float)*3);
	node->rotation = (float*)malloc(sizeof(float)*3);

	node->translation[NODE_X] = 0.0f;
	node->translation[NODE_Y] = 0.0f;
	node->translation[NODE_Z] = 0.0f;

	node->rotation[NODE_X] =  0.0f;
	node->rotation[NODE_Y] =  0.0f;
	node->rotation[NODE_Z] =  0.0f;
	return;
}

int dsgNodeHasValidPath(node_t* node) {
	if (node == NULL) return 0;
	return node->path != NULL;
}

int dsgNodeHasValidName(node_t* node) {
	if (node == NULL) return 0;
	return node->name != NULL;
}

void dsgNodePrint(node_t* node, void* arg) {
	fprintf(stdout,"%s\n", node->path);
	return;
}

void dsgNodeSetParentIndex(int parent, node_t* node) {
	node->parentIndex = parent;
	return;
}

void dsgNodeSetTranslation(node_t* node, float x, float y, float z) {
	node->translation[NODE_X] = x;
	node->translation[NODE_Y] = y;
	node->translation[NODE_Z] = z;
	return;
}

void dsgNodeSetRotation(node_t* node, float x, float y, float z) {
	node->rotation[NODE_X] = x;
	node->rotation[NODE_Y] = y;
	node->rotation[NODE_Z] = z;
	return;
}

int dsgNodeHasVertexBuffer(node_t* node) {
	return node->vertexBufferIndex > -1;
}
void dsgNodeSetPolarTranslation(node_t* node, node_t* parent,float radius, float theta) {
	node->rotation[NODE_X] = node->rotation[NODE_X];
	node->rotation[NODE_Y] = node->rotation[NODE_Y];
	node->rotation[NODE_Z] = node->rotation[NODE_Z];
	return;
}
