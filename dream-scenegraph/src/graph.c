/*
* graph.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "node.h"
#include "graph.h"

char dsgGraphPathDelimeter  = '/';

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
	retval->name          =  NULL;
	retval->rootNodeIndex = -1;
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
	int i;
	for (i=0; i<NODES; i++) {
		if (graph->nodes[i] == NULL) {
			return i;
		}
	}
	return -1;
}

int dsgGraphGetIndexOfNode(graph_t *graph, node_t *node) {
	int i;
	for (i=0; i<NODES; i++) {
		if (graph->nodes[i] == node) {
			return i;
		}
	}
	return -1;
}

node_t* dsgGraphGetRootNode(graph_t* graph) {
	return graph->nodes[graph->rootNodeIndex];
}

void dsgGraphSetRootNode(graph_t* graph, node_t* root) {
	if (graph == NULL) {
		fprintf(stderr,"Cannot set root node of NULL graph.\n");
		return;
	}
	graph->rootNodeIndex = dsgGraphGetIndexOfNode(graph,root);
	return;
}

void dsgGraphSetRootNodeIndex(graph_t* graph, int root) {
	if (graph == NULL) {
		fprintf(stderr,"Cannot set root node of NULL graph\n");
		return;
	}
	graph->rootNodeIndex = root;
	return;
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
	return;
}

void dsgGraphTraverseNodeVector(graph_t* graph, void (*function)(node_t*, void*), void* arg) {
	int i;
	node_t* next;
	for (i=0;i<NODES;i++) {
		next = graph->nodes[i];
		if (next == NULL) {
			continue;
		}
		(*function)((void*)next, arg == NULL ? graph : arg);
	}
	return;
}

node_t* dsgGraphGetNodeByName(graph_t* graph, char* name) {
	int i;
	node_t* next;
	for (i=0;i<NODES;i++) {
		next = graph->nodes[i];
		if (next == NULL) {
			continue;
		}	
		if (next->name == NULL) {
			fprintf(stderr,"dsgGraphGetNodeByName: Cannot test node with NULL name\n");
			continue;
		}
		if (strcmp(graph->nodes[i]->name,name) == 0) {
			return graph->nodes[i];
		}	
	}
	return NULL;
}

int dsgGraphTraversePath(graph_t* graph, char* path, void (*function)(node_t*, void*), void* arg) {
	char* start    = NULL;
	char* end      = NULL;
	int done       = 0;
	int nameLen    = 0;
	node_t* target = NULL;
	//fprintf(stdout,"Traversing path: %s\n",path);
	start = path+1;
	if (start == NULL) {
		fprintf(stderr,"Path is null\n");
		return done;
	}
	while (!done) {
		nameLen = 0;
		end = strchr(start+1, dsgGraphPathDelimeter);
		if (end == NULL) {
			//fprintf(stdout,"Found end of string\n");
			end = path+strlen(path); 
			done = 1;
		}
		end += 1;
		nameLen = (end-start-1);
		if (nameLen <= 0) {
			fprintf(stderr,"Traverse nameLen <= 0\n");
			break;
		}
		char* tmp = (char*)malloc(sizeof(char)*nameLen+1);
		memcpy(tmp,start,nameLen);
		tmp[nameLen] = '\0';
		//fprintf(stdout,"Traversed path to: %s\n",tmp);
		target = dsgGraphGetNodeByName(graph,tmp);
		if (target != NULL) {
			function(target,arg);
			start = end;
		} else {
			fprintf(stderr,"%s not found while traversing path\n",tmp);
			break;
		}
	}
	return done;
}

int dsgGraphCountChildrenOfNode(graph_t* graph, node_t* node) {
	int retval = 0;
	int i;
	node_t* next;
	for (i = 0; i<NODES; i++) {
		next = graph->nodes[i];
		if (next == NULL) {
			continue;
		}

		if (dsgGraphNodeIsChildOf(graph, node, next)) {
			retval++;
		}
	}
	return retval;
}

void dsgGraphUpdatePaths(graph_t* graph) {
	int i;
	node_t* next;
	for (i=0;i<NODES;i++) {
		next = graph->nodes[i];	
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
		node->path = NULL;
	}

	char* pathBuffer = (char*)malloc(sizeof(char)*STR_BUF_SIZE);
	// Node has no parent
	if (node->parentIndex == -1) {
		// and is not graph root
		if (!dsgGraphIsRootNode(graph,node)) {
			fprintf(stderr, "Cannot generate path for non-root node with no parent.\n");	
			return;
		}
		fprintf(stdout,"Generating path for node root %s\n", node->name);
		memcpy(pathBuffer, &dsgGraphPathDelimeter, 1);	
		pathBuffer[2] = '\0';
		strncat(pathBuffer, node->name, strlen(node->name));
	} else {
		node_t* parent = graph->nodes[node->parentIndex];
		if (parent == NULL) {
			fprintf(stderr,"Error finding node parent to create path\n");
			return;
		}
		fprintf(stdout,"Generating path for node %s with parent %s\n", node->name, parent->name);
		memcpy(pathBuffer, parent->path, strlen(parent->path)+1);
		strncat(pathBuffer, &dsgGraphPathDelimeter, strlen(&dsgGraphPathDelimeter));	
		strncat(pathBuffer, node->name, strlen(node->name));	
	}

	// Trim buffer fat
	int compressedLength =  strlen(pathBuffer)+1;
	char* pathBufferCompressed = (char*)malloc(sizeof(char)*compressedLength);
	memcpy(pathBufferCompressed,pathBuffer,compressedLength);
	pathBufferCompressed[compressedLength-1] = '\0';
	node->path = pathBufferCompressed;
	free(pathBuffer);

	fprintf(stdout,"Generated path: %s\n",node->path);
	return;
}

int dsgGraphIsRootNode(graph_t* graph, node_t* node) {
	return graph->rootNodeIndex == dsgGraphGetIndexOfNode(graph,node);
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

int dsgGraphNodeIsParentOf (graph_t* graph, node_t* parent, node_t* child) {
	int retval = 0; 
	if (parent < 0 || !dsgNodeHasValidName(child) || !dsgNodeHasValidPath(child)) {
		return retval;	
	}
	retval = child->parentIndex == dsgGraphGetIndexOfNode(graph,parent);
	return retval;
}

int dsgGraphNodeIsChildOf(graph_t* graph, node_t* parent, node_t* child) {
	if (parent == NULL || child == NULL || 
	    !dsgNodeHasValidPath(child) || 
	    !dsgNodeHasValidName(child)) {
		return 0;	
	}
	return child->parentIndex == dsgGraphGetIndexOfNode(graph,parent);
}

void dsgGraphNodeSetParent (graph_t* graph, node_t* parent, node_t* node) {
	dsgNodeSetParentIndex(dsgGraphGetIndexOfNode(graph,parent),node);
	return;
}

void dsgGraphPrintLine(node_t* node, void* arg) {
	fprintf(stdout,"%s\n",node->path);
	return;
}

void dsgGraphPrintGraph(graph_t* graph) {
	dsgGraphTraverseNodeVector(graph,&dsgGraphPrintLine,NULL);
	return;
}
