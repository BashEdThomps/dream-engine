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
	for (i=0;i<MAX_NODES;i++) {
		retval->nodes[i] = NULL;	
	}
	retval->name     = NULL;
	retval->rootNode = NULL;
	return retval;
}

node_t* dsgGraphCreateNodeWithName(graph_t* graph, char* name) {
	node_t* retval = dsgGraphCreateNode(graph);
	retval->name = name;
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
	for (i=0; i<MAX_NODES; i++) {
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
	for (i=0; i<MAX_NODES; i++) {
		if (graph->nodes[i] == node) {
			retval = i;
			break;
		}
	}
	return retval;
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
		fprintf(stderr,"Cannot find node in graph to remove it.");
		return;
	}

	free(node);
	graph->nodes[i] = NULL; 
}

void dsgGraphPrintGraph(graph_t* graph) {
	dsgGraphPrintGraphFromNode(graph,graph->rootNode);
	return;
}

void dsgGraphPrintGraphFromNode(graph_t* graph, node_t* node) {
	fprintf(stderr,"dsgGraphPrintGraphFromNode: Not Yet Implemented\n");
	return;
}

void dsgGraphUpdateTraRots(graph_t* graph) {
	int i;
	node_t* next;
	for (i=0;i<MAX_NODES;i++) {
		next = graph->nodes[i];		
		if (next->parent != NULL) {
			dsgNodeSumTranslationRotation(next);	
		}
	}
}

void dsgGraphUpdateChildren(graph_t* graph) {
	int i;
	node_t* node;
	for (i=0;i<MAX_NODES;i++) {
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

void dsgGraphPopulateChildren(graph_t* graph, node_t* node) {
	node_t* next;
	int i;
	int childrenFound = 0;
	for (i=0; i<MAX_NODES, childrenFound < node->numChildren; i++) {
		next = graph->nodes[i];
		if (dsgNodeIsChildOf(node, next)) {
			node_t** childrenArray = (node_t**)node->children;
			childrenArray[childrenFound] = next;	
			childrenFound++;
		}				
	} 
}

int dsgGraphCountChildrenOfNode(graph_t* graph, node_t* node) {
	int retval = 0;
	int i;
	for (i=0;i<MAX_NODES;i++) {
		node_t* next = graph->nodes[i];
		if (dsgNodeIsChildOf(node,next)) {
			retval++;
		}
	}
	return retval;
}

void dsgGraphUpdatePaths(graph_t* graph) {
	int i;
	for (i=0;i<MAX_NODES;i++) {
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
	dsgGraphUpdateTraRots  (graph);
}
