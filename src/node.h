#ifndef NODE_H
#define NODE_H

/*
* node.h
*/
#include "node_trarot.h"

typedef struct {
	char *name;
	char *path;
	int   parentIndex;
	int   numChildren;
	int   numImmediateChildren;
	int  *children;
	int  *immediateChildren;
	nodeTraRot_t *localTraRot;
	nodeTraRot_t *globalTraRot;
} node_t;

typedef struct {
	node_t* node;
	node_t* parent;
} dsgNodeSumWithParentArg_t;

// Init
void dsgNodeInit (node_t*, char*);
void dsgNodeInitTranslationRotation  (node_t*);

// Validation
int dsgNodeHasValidName(node_t*);
int dsgNodeHasValidPath(node_t*);

// Math
void dsgNodeSumWithParentTranslationRotation(void*);

// Debug
void dsgNodePrint(node_t*, void*);

// Relationships
void dsgNodeSetParentIndex(int, node_t*);
#endif // NODE_H
