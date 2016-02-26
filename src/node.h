#ifndef NODE_H
#define NODE_H

/*
* node.h
*/
#include "node_trarot.h"

typedef struct {
	char *name;
	char *path;
	void *parent;
	int   numChildren;
	void *children;
	nodeTraRot_t *localTraRot;
	nodeTraRot_t *globalTraRot;
} node_t;

// Init
void dsgNodeInit       (node_t*, char*);
void dsgNodeIniTraRot  (node_t*);

// Relationships
void dsgNodeSetParent (node_t*, node_t*);
int dsgNodeIsChildOf  (node_t*, node_t*);
int dsgNodeIsParentOf (node_t*, node_t*);

// Validation
int dsgNodeHasValidName (node_t*);
int dsgNodeHasValidPath (node_t*);

// Math
dsgNodeSumWithParentTraRot(node_t*);

#endif // NODE_H
