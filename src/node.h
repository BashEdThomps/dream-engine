#ifndef NODE_H
#define NODE_H
/*
* node.h
*/
typedef struct {
	float transX;
	float transY;
	float transZ;
	float rotX;
	float rotY;
	float rotZ;
} nodeTransRot_t;

typedef struct {
	char *name;
	char *path;
	void *parent;
	int   numChildren;
	void *children;
	nodeTransRot_t *transRot;
} node_t;

// Init
void dsgNodeInit (node_t*, char*);
void dsgNodeInitTransRot(node_t*);

// Relationships
void dsgNodeSetParent (node_t*, node_t*);
int dsgNodeIsChildOf  (node_t*, node_t*);
int dsgNodeIsParentOf (node_t*, node_t*);

// Validation
int dsgNodeHasValidName(node_t*);
int dsgNodeHasValidPath(node_t*);

#endif // NODE_H
