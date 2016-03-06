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
} dsgNode;

void dsgNodeInit (dsgNode*, char*);
void dsgNodeInitTranslationRotation(dsgNode*);
void dsgNodeDestroy(dsgNode*);
int dsgNodeHasValidName(dsgNode*);
int dsgNodeHasValidPath(dsgNode*);
void dsgNodePrint(dsgNode*, void*);
void dsgNodeSetParentIndex(int, dsgNode*);
void dsgNodeSetTranslation(dsgNode*, float, float, float);
void dsgNodeSetRotation(dsgNode*, float, float, float);
void dsgNodeSetPolarTranslation(dsgNode*, dsgNode*, float, float);
int dsgNodeHasVertexBuffer(dsgNode*);

#endif // NODE_H
