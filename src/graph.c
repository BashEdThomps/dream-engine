/*
* graph.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "node.h"
#include "graph.h"

void dsgGraphInitWithName(graph_t* graph, char* name) {
	dsgGraphInit(graph);
	graph->name = name;
}

void dsgGraphInit(graph_t* graph) {
	graph = (graph_t*)malloc(sizeof(graph_t));
	int i;
	for (i=0;i<MAX_NODES;i++) {
		graph->nodes[i] = NULL;	
	}
	graph->name     = NULL;
	graph->rootNode = NULL;
	return;
}

void dsgGraphCreateNodeWithName(graph_t* graph, node_t* node, char* name) {
	node = (node_t*)malloc(sizeof(node_t));
	dsgGraphCreateNode(graph, node);
	node->name = name;
}

void dsgGraphCreateNode(graph_t* graph, node_t* node) {
	int available = dsgGraphGetNextAvailableNode(graph);
	if (available < 0) {
		fprintf(stderr,
			   "Error: Unable to create new node - tree is full\n");
		return;
	}

	node = (node_t*)malloc(sizeof(node_t));
	dsgNodeInit(node, NULL);
	graph->nodes[available] = node;
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
	return;
}

void dsgGraphUpdate(graph_t* graph) {
	int i;
	for (i=0;i<MAX_NODES;i++) {
		node_t* next = graph->nodes[i];	
		if (next != NULL) {
			dsgGraphGeneratePathForNode(graph, next);
		} else {
			fprintf(stdout,"Skip update for NULL node %d\n",i);
		}
	}
	return;
}

void dsgGraphGeneratePathForNode(graph_t* graph, node_t* node) {
	if (node->name == NULL) {
		fprintf(stderr,"Cannot generate path for node with no name\n");	
		return;
	}

	if (node->parent == NULL) {
		fprintf(stderr,"Cannot generate path for node with no name\n");	
		return;
	}

	fprintf(stdout,"Generating path for node %s with parent %s\n",
			node->name, ((node_t*)node->parent)->name);

	if (node->path != NULL) {
		free(node->path);	
		node->path == NULL;
	}

	node_t* next;

	char pathBuffer[STR_BUF_SIZE];
	size_t nameLen =  strlen(node->name);

	memcpy(pathBuffer,node->name,nameLen);

	pathBuffer[nameLen] = '\0';

	for (/*nout*/; next != NULL; next = node->parent) {
		strncat(pathBuffer, "/", 1);	
		strncat(pathBuffer, next->name, strlen(next->name));	
	}

	node->path = pathBuffer;
	fprintf(stdout,"Generated path: %s\n",node->path);
	return;
}
