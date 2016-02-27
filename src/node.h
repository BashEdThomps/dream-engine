#ifndef NODE_H
#define NODE_H

/*
* node.h
*/
#include "node_trarot.h"

typedef struct _node_t{
	char *name;
	char *path;
	struct _node_t *parent;
	int   numChildren;
	int   numImmediateChildren;
	struct _node_t **children;
	struct _node_t **immediateChildren;
	nodeTraRot_t *localTraRot;
	nodeTraRot_t *globalTraRot;
} node_t;

// Init
void dsgNodeInit       (node_t*, char*);
void dsgNodeInitTranslationRotation  (node_t*);

// Relationships
void dsgNodeSetParent (node_t*, node_t*);
int  dsgNodeIsChildOf  (node_t*, node_t*);
int  dsgNodeIsImmediateChildOf  (node_t*, node_t*);
int  dsgNodeIsParentOf (node_t*, node_t*);

// Validation
int dsgNodeHasValidName (node_t*);
int dsgNodeHasValidPath (node_t*);

// Math
void dsgNodeSumWithParentTranslationRotation(node_t*);

#endif // NODE_H
