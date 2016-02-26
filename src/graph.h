#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"

#define MAX_NODES 256
#define STR_BUF_SIZE 512

// Global Variables
typedef struct {
	char*   name;
	node_t* nodes[MAX_NODES];
	node_t* rootNode;
} graph_t;

// Init
graph_t* dsgGraphInit();
graph_t* dsgGraphInitWithName(char*);
int dsgGraphGetNextAvailableNode(graph_t*);
int dsgGraphGetIndexOfNode(graph_t*, node_t*);

// Display/Debug
void dsgGraphPrintGraph(graph_t*);
void dsgGraphPrintGraphFromNode(graph_t*, node_t*);

// Path
void dsgGraphGeneratePathForNode(graph_t*, node_t*);

// Node Management
void dsgGraphSetRootNode(graph_t*, node_t*); 
int dsgGraphIsRootNode(graph_t*, node_t*);
node_t* dsgGraphCreateNode(graph_t*);
node_t* dsgGraphCreateNodeWithName(graph_t*,char*);
void dsgGraphRemoveNode(graph_t*, node_t*);
int dsgGraphCountChildrenOfNode(graph_t*, node_t*);
void dsgGraphPopulateChildren(graph_t*, node_t*);

// Updating
void dsgGraphUpdateAll(graph_t*);
void dsgGraphUpdatePaths(graph_t*);
void dsgGraphUpdateOffsets(graph_t*);
void dsgGraphUpdateChildren(graph_t*);

#endif // GRAPH_H
