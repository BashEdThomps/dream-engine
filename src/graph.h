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
void dsgGraphInit(graph_t*);
void dsgGraphInitWithName(graph_t*, char*);
int dsgGraphGetNextAvailableNode(graph_t*);
int dsgGraphGetIndexOfNode(graph_t*, node_t*);

// Display/Debug
void dsgGraphPrintGraph(graph_t*);
void dsgGraphPrintGraphFromNode(graph_t*, node_t*);

// Path
void dsgGraphGeneratePathForNode(graph_t*, node_t*);

// Node Management
void dsgGraphSetRootNode(graph_t*, node_t*); 
void dsgGraphCreateNode(graph_t*, node_t*);
void dsgGraphCreateNodeWithName(graph_t*, node_t*,char*);
void dsgGraphRemoveNode(graph_t*, node_t*);

// Updating
void dsgGraphUpdate(graph_t*);

#endif // GRAPH_H
