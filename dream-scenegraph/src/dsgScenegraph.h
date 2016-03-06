#ifndef GRAPH_H
#define GRAPH_H

#include "dsgNode.h"
#include "dsgCamera.h"

#define NODES        256
#define VERT_BUFFERS 256
#define STR_BUF_SIZE 512


// Global Variables
typedef struct {
	char*      name;
	dsgNode*   nodes         [NODES];
	float*     vertexBuffers [VERT_BUFFERS];
	dsgCamera* camera;
	int        rootNodeIndex;
} dsgScenegraph;

// Init
dsgScenegraph* dsgScenegraphInit();
dsgScenegraph* dsgScenegraphInitWithName(char*);

// VertexBuffer
int dsgScenegraphAddVertexBuffer              (dsgScenegraph*, void*);
int dsgScenegraphGetNextAvailableVertexBuffer (dsgScenegraph*);
int dsgScenegraphGetIndexOfVertexBuffer       (dsgScenegraph*, void*);
int dsgScenegraphRemoveVertexBuffer           (dsgScenegraph*, void*);

// Display/Debug
void dsgScenegraphPrintGraph (dsgScenegraph*);

// Node Management
void    dsgScenegraphGeneratePathForNode  (dsgScenegraph*, dsgNode*);
void    dsgScenegraphSetRootNodeIndex     (dsgScenegraph*, int);
void    dsgScenegraphSetRootNode          (dsgScenegraph*, dsgNode*);
int     dsgScenegraphGetNextAvailableNode (dsgScenegraph*);
int     dsgScenegraphGetIndexOfNode       (dsgScenegraph*, dsgNode*);
dsgNode* dsgScenegraphGetRootNode                  (dsgScenegraph*);
int     dsgScenegraphGetRootNodeIndex             (dsgScenegraph*);
int     dsgScenegraphIsRootNode                   (dsgScenegraph*, dsgNode*);
dsgNode* dsgScenegraphCreateNode                   (dsgScenegraph*);
dsgNode* dsgScenegraphCreateNodeWithName           (dsgScenegraph*, char*);
void    dsgScenegraphRemoveNode                   (dsgScenegraph*, dsgNode*);
int     dsgScenegraphCountChildrenOfNode          (dsgScenegraph*, dsgNode*);
dsgNode* dsgScenegraphGetNodeByName                (dsgScenegraph*, char*);

// Updating
void dsgScenegraphUpdatePaths               (dsgScenegraph*);
void dsgScenegraphTraverseNodeVector        (dsgScenegraph*, void (*)(dsgNode*, void*), void*);
int  dsgScenegraphTraversePath              (dsgScenegraph*,char*,void (*)(dsgNode*,void*),void*);

// Node Relationships
int  dsgScenegraphNodeIsChildOf           (dsgScenegraph*, dsgNode*, dsgNode*);
int  dsgScenegraphNodeIsParentOf          (dsgScenegraph*, dsgNode*, dsgNode*);
void dsgScenegraphNodeSetParent           (dsgScenegraph*, dsgNode*, dsgNode*);

void dsgScenegraphPrintLine(dsgNode* node, void* arg);
void dsgScenegraphPrintGraph(dsgScenegraph* graph);

#endif // GRAPH_H
