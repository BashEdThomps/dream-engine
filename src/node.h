#ifndef NODE_H
#define NODE_H

/*
* node.h
*/

#define NODE_X 0
#define NODE_Y 1
#define NODE_Z 2

typedef struct {
	char  *name;
	char  *path;
	int    parentIndex;
	int    vertexBufferIndex;
	float *translation; 	
	float * rotation;
} node_t;

void dsgNodeInit (node_t*, char*);
void dsgNodeInitTranslationRotation(node_t*);
void dsgNodeDestroy(node_t*);
int dsgNodeHasValidName(node_t*);
int dsgNodeHasValidPath(node_t*);
void dsgNodePrint(node_t*, void*);
void dsgNodeSetParentIndex(int, node_t*);
void dsgNodeSetTranslation(node_t*, float, float, float);
void dsgNodeSetRotation(node_t*, float, float, float);
void dsgNodeSetPolarTranslation(node_t*, node_t*, float, float);
int dsgNodeHasVertexBuffer(node_t*);

#endif // NODE_H
