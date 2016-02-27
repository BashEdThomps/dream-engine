/*
* graph.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "node.h"
#include "graph.h"

graph_t* dsgGraphInitWithName(char* name) {
	graph_t* retval = dsgGraphInit();
	retval->name = name;
	return retval;
}

graph_t* dsgGraphInit() {
	graph_t* retval = (graph_t*)malloc(sizeof(graph_t));
	int i;
	for (i=0;i<NODES;i++) {
		retval->nodes[i] = NULL;	
	}
	retval->name     = NULL;
	retval->rootNode = NULL;
	return retval;
}

node_t* dsgGraphCreateNodeWithName(graph_t* graph, char* name) {
	node_t* retval = dsgGraphCreateNode(graph);
	retval->name = name;
	dsgNodeInitTranslationRotation(retval);
	return retval;
}

node_t* dsgGraphCreateNode(graph_t* graph) {
	node_t* retval = NULL;
	int available = dsgGraphGetNextAvailableNode(graph);
	if (available < 0) {
		fprintf(stderr,
			   "Error: Unable to create new node - tree is full\n");
		return NULL;
	}
	retval = (node_t*)malloc(sizeof(node_t));
	dsgNodeInit(retval, NULL);
	graph->nodes[available] = retval;
	return retval;
}

int dsgGraphGetNextAvailableNode(graph_t* graph) {
	int retval = -1;
	int i;
	for (i=0; i<NODES; i++) {
		if (graph->nodes[i] == NULL) {
			retval = i;
			break;
		}
	}
	return retval;
}

int dsgGraphGetIndexOfNode(graph_t *graph, node_t *node) {
	int retval = -1;
	int i;
	for (i=0; i<NODES; i++) {
		if (graph->nodes[i] == node) {
			retval = i;
			break;
		}
	}
	return retval;
}

node_t* dsgGraphGetRootNode(graph_t* graph) {
	return graph->rootNode;
}

void dsgGraphSetRootNode(graph_t* graph, node_t* root) {
	if (graph == NULL) {
		fprintf(stderr,"Cannot set root node of NULL graph.\n");
		return;
	}
	graph->rootNode = root;
}

void dsgGraphRemoveNode(graph_t* graph, node_t* node) {

	if (graph == NULL) {
		fprintf(stderr,"Cannot remove node from NULL graph.\n");
		return;
	}

	if (node == NULL) {
		fprintf(stderr,"Cannot remove NULL node from graph.\n");
		return;
	}

	int i = dsgGraphGetIndexOfNode(graph, node);
	if (i < 0) {
		fprintf(stderr,"Cannot find node in graph to remove it.\n");
		return;
	}

	free(node);
	graph->nodes[i] = NULL; 
}

void dsgGraphPrintGraph(graph_t* graph) {
	fprintf(stdout,"Printing Graph %s\n",graph->name ? graph->name : "Untitled Graph");
	node_t* start = dsgGraphGetRootNode(graph);
	fprintf(stdout,"%s\n", start->path);
	node_t* next = NULL;
	int i;
	for (i=0; i<start->numChildren; i++) {
		next = (node_t*)start->children[i];
		fprintf(stdout,"%s\n",next->path);
	} 
	return;
}

void dsgGraphUpdateTranslationRotation(graph_t* graph) {
	int i;
	node_t* next;
	for (i=0;i<NODES;i++) {
		if (next == NULL) {
			continue;	
		}
		next = graph->nodes[i];		
		if (next->parent != NULL) {
			dsgNodeSumWithParentTranslationRotation(next);	
		}
	}
}

void dsgGraphUpdateChildren(graph_t* graph) {
	int i;
	node_t* node;
	for (i=0;i<NODES;i++) {
		node = graph->nodes[i];	
		if (node == NULL) {
			continue;
		}

		if (node->children != NULL) {
			free(node->children);
		}

		int numChildren = dsgGraphCountChildrenOfNode(graph, node);
		node->numChildren = numChildren;
		node->children = (void*)malloc(sizeof(node_t*)*numChildren);
		dsgGraphPopulateChildren(graph, node);
	}
	return;
}

void dsgGraphUpdateImmediateChildren(graph_t* graph) {
	int i;
	node_t* node;
	for (i=0;i<NODES;i++) {
		node = graph->nodes[i];	

		if (node == NULL) {
			continue;
		}

		if (node->children != NULL) {
			free(node->children);
		}

		int numImmediateChildren = dsgGraphCountImmediateChildrenOfNode(graph, node);
		node->numImmediateChildren = numImmediateChildren;
		node->immediateChildren = (void*)malloc(sizeof(node_t*)*numImmediateChildren);
		dsgGraphPopulateImmediateChildren(graph, node);
	}
	return;
}

void dsgGraphPopulateChildren(graph_t* graph, node_t* node) {
	node_t* next;
	int i;
	int childrenFound = 0;
	for (i=0; i<NODES, childrenFound < node->numChildren; i++) {
		next = graph->nodes[i];
		if (dsgNodeIsChildOf(node, next)) {
			node_t** childrenArray = (node_t**)node->children;
			childrenArray[childrenFound] = next;	
			childrenFound++;
		}				
	} 
}

void dsgGraphPopulateImmediateChildren(graph_t* graph, node_t* node) {
	node_t* next;
	int i;
	int childrenFound = 0;
	for (i=0; i<NODES, childrenFound < node->numImmediateChildren; i++) {
		next = graph->nodes[i];
		if (dsgNodeIsImmediateChildOf(node, next)) {
			node_t** childrenArray = (node_t**)node->children;
			childrenArray[childrenFound] = next;	
			childrenFound++;
		}				
	} 
}

int dsgGraphCountChildrenOfNode(graph_t* graph, node_t* node) {
	int retval = 0;
	int i;
	for (i=0;i<NODES;i++) {
		node_t* next = graph->nodes[i];
		if (dsgNodeIsChildOf(node,next)) {
			retval++;
		}
	}
	return retval;
}

int dsgGraphCountImmediateChildrenOfNode(graph_t* graph, node_t* node) {
	int retval = 0;
	int i;
	for (i=0;i<NODES;i++) {
		node_t* next = graph->nodes[i];
		if (dsgNodeIsImmediateChildOf(node,next)) {
			retval++;
		}
	}
	return retval;
}

void dsgGraphUpdatePaths(graph_t* graph) {
	int i;
	for (i=0;i<NODES;i++) {
		node_t* next = graph->nodes[i];	
		if (next != NULL) {
			dsgGraphGeneratePathForNode(graph, next);
		} 
	}
	return;
}

void dsgGraphGeneratePathForNode(graph_t* graph, node_t* node) {
	if (node == NULL) {
		fprintf(stderr,"Cannot generate path for NULL node.\n");	
		return;
	}

	if (graph == NULL) {
		fprintf(stderr,"Cannot generate path for NULL graph.\n");	
		return;
	}

	if (node->name == NULL) {
		fprintf(stderr,"Cannot generate path for node with no name.\n");	
		return;
	}

	if (node->path != NULL) {
		free(node->path);	
		node->path == NULL;
	}

	char* pathBuffer = (char*)malloc(sizeof(char)*STR_BUF_SIZE);

	if (node->parent == NULL) {
		if (!dsgGraphIsRootNode(graph,node)) {
			fprintf(stderr, "Cannot generate path for non-root node with no parent.\n");	
			return;
		}
		fprintf(stdout,"Generating path for node root %s\n", node->name);
		strncat(pathBuffer, "/", 1);	
		strncat(pathBuffer, node->name, strlen(node->name));
	} else {
		node_t* parent = (node_t*) node->parent;
		fprintf(stdout,"Generating path for node %s with parent %s\n", node->name, parent->name);
		strncat(pathBuffer, parent->path, strlen(parent->path));
		strncat(pathBuffer, "/", 1);	
		strncat(pathBuffer, node->name, strlen(node->name));	
	}
	node->path = pathBuffer;
	fprintf(stdout,"Generated path: %s\n",node->path);
	return;
}

int dsgGraphIsRootNode(graph_t* graph, node_t* node) {
	return graph->rootNode == node;
}

void dsgGraphUpdateAll(graph_t* graph) {
	dsgGraphUpdatePaths    (graph);
	dsgGraphUpdateChildren (graph);
	dsgGraphUpdateImmediateChildren    (graph);
	dsgGraphUpdateTranslationRotation  (graph);
}

int dsgGraphAddVertexBuffer(graph_t* graph, void* buffer) {
	fprintf(stdout,"Adding vertex buffer to graph\n");
	int index = dsgGraphGetNextAvailableVertexBuffer(graph);
	if (index < 0) {
		fprintf(stderr,"Unable to add vertex buffer, graph is full\n");
		return -1;
	}
	graph->vertexBuffers[index] = (float*)buffer;
	return index;
}

int  dsgGraphGetNextAvailableVertexBuffer(graph_t* graph) {
	if (graph == NULL) {
		return -1;
	}

	int retval = -1;
	int i;
	for (i=0; i<VERT_BUFFERS; i++) {
		if (graph->vertexBuffers[i] == NULL) {
			retval = i;
			break;
		}
	}
	return retval;
}

int  dsgGraphGetIndexOfVertexBuffer(graph_t* graph, void* buffer) {
	if(graph == NULL || buffer == NULL) {
		return -1;
	}

	int retval = -1;
	int i;
	for (i=0; i<VERT_BUFFERS; i++) {
		if (graph->vertexBuffers[i] == buffer) {
			retval = i;
			break;
		}
	}
	return retval;
}

int dsgGraphRemoveVertexBuffer(graph_t* graph, void* buffer) {
	if (graph == NULL || buffer == NULL) {
		return -1;	
	}

	int index = dsgGraphGetIndexOfNode(graph,buffer);
	if (index < 0) {
		fprintf(stderr,"Cannot remove vertex buffer from graph %s, not found\n",
				                graph->name != NULL ? graph->name : "Unamed Graph");
		return -1;
	}

	free(graph->vertexBuffers[index]);
	graph->vertexBuffers[index] = NULL;
	return index;
}


