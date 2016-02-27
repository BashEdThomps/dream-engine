#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"

#define NODES        256
#define VERT_BUFFERS 256
#define STR_BUF_SIZE 512

// Global Variables
typedef struct {
	char*   name;
	node_t* nodes         [NODES];
	int     orderedNodes  [NODES];
	float*  vertexBuffers [VERT_BUFFERS];
	int     rootNodeIndex;
} graph_t;

// Init
graph_t* dsgGraphInit();
graph_t* dsgGraphInitWithName(char*);

// VertexBuffer
int dsgGraphAddVertexBuffer              (graph_t*, void*);
int dsgGraphGetNextAvailableVertexBuffer (graph_t*);
int dsgGraphGetIndexOfVertexBuffer       (graph_t*, void*);
int dsgGraphRemoveVertexBuffer           (graph_t*, void*);

// Display/Debug
void dsgGraphPrintGraph (graph_t*);

// Node Management
void    dsgGraphGeneratePathForNode  (graph_t*, node_t*);
void    dsgGraphSetRootNodeIndex     (graph_t*, int); 
void    dsgGraphSetRootNode          (graph_t*, node_t*); 
int     dsgGraphGetNextAvailableNode (graph_t*);
int     dsgGraphGetIndexOfNode       (graph_t*, node_t*);
int     dsgGraphGetNextAvailableOrderedNodeIndex (graph_t*);
node_t* dsgGraphGetRootNode                  (graph_t*);
int     dsgGraphGetRootNodeIndex             (graph_t*);
int     dsgGraphIsRootNode                   (graph_t*, node_t*);
node_t* dsgGraphCreateNode                   (graph_t*);
node_t* dsgGraphCreateNodeWithName           (graph_t*,char*);
void    dsgGraphRemoveNode                   (graph_t*, node_t*);
int     dsgGraphCountChildrenOfNode          (graph_t*, node_t*);
int     dsgGraphCountImmediateChildrenOfNode (graph_t*, node_t*);
void    dsgGraphPopulateChildren             (graph_t*, node_t*);
void    dsgGraphPopulateImmediateChildren    (graph_t*, node_t*);

// Updating
void dsgGraphUpdateAll                 (graph_t*);
void dsgGraphUpdatePaths               (graph_t*);
void dsgGraphUpdateTranslationRotation (graph_t*);
void dsgGraphUpdateChildren            (graph_t*);
void dsgGraphUpdateImmediateChildren   (graph_t*);
void dsgGraphUpdateOrder               (graph_t*);
void dsgGraphUpdateOrderFromNode       (graph_t*,node_t*);
void dsgGraphInitOrderedNodes         (graph_t*);
void dsgGraphTraverseOrderedNodes(graph_t*, void (*)(node_t*, void*), void*); 
// Node Relationships
int  dsgGraphNodeIsChildOf           (graph_t*, node_t*, node_t*);
int  dsgGraphNodeIsImmediateChildOf  (graph_t*, node_t*, node_t*);
int  dsgGraphNodeIsParentOf          (graph_t*, node_t*, node_t*);
int  dsgGraphNodeIsImmediateParentOf (graph_t*, node_t*, node_t*);
void dsgGraphNodeSetParent           (graph_t*, node_t*, node_t*);

#endif // GRAPH_H
