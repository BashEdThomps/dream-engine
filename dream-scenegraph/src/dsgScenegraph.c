/*
* graph.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dsgNode.h"
#include "dsgScenegraph.h"

char dsgScenegraphPathDelimeter  = '/';

dsgScenegraph* dsgScenegraphInitWithName(char* name) {
	dsgScenegraph* retval = dsgScenegraphInit();
	retval->name = name;
	return retval;
}

dsgScenegraph* dsgScenegraphInit() {
	dsgScenegraph* retval = (dsgScenegraph*)malloc(sizeof(dsgScenegraph));
	int i;
	for (i=0;i<NODES;i++) {
		retval->nodes[i] = NULL;
	}
	retval->camera = dsgCameraInit();
	retval->name          =  NULL;
	retval->rootNodeIndex = -1;
	return retval;
}

dsgNode* dsgScenegraphCreateNodeWithName(dsgScenegraph* graph, char* name) {
	dsgNode* retval = dsgScenegraphCreateNode(graph);
	retval->name = name;
	return retval;
}

dsgNode* dsgScenegraphCreateNode(dsgScenegraph* graph) {
	dsgNode* retval = NULL;
	int available = dsgScenegraphGetNextAvailableNode(graph);
	if (available < 0) {
		fprintf(stderr,
			   "Error: Unable to create new node - tree is full\n");
		return NULL;
	}
	retval = (dsgNode*)malloc(sizeof(dsgNode));
	dsgNodeInit(retval, NULL);
	graph->nodes[available] = retval;
	return retval;
}

int dsgScenegraphGetNextAvailableNode(dsgScenegraph* graph) {
	int i;
	for (i=0; i<NODES; i++) {
		if (graph->nodes[i] == NULL) {
			return i;
		}
	}
	return -1;
}

int dsgScenegraphGetIndexOfNode(dsgScenegraph *graph, dsgNode *node) {
	int i;
	for (i=0; i<NODES; i++) {
		if (graph->nodes[i] == node) {
			return i;
		}
	}
	return -1;
}

dsgNode* dsgScenegraphGetRootNode(dsgScenegraph* graph) {
	return graph->nodes[graph->rootNodeIndex];
}

void dsgScenegraphSetRootNode(dsgScenegraph* graph, dsgNode* root) {
	if (graph == NULL) {
		fprintf(stderr,"Cannot set root node of NULL graph.\n");
		return;
	}
	graph->rootNodeIndex = dsgScenegraphGetIndexOfNode(graph,root);
	return;
}

void dsgScenegraphSetRootNodeIndex(dsgScenegraph* graph, int root) {
	if (graph == NULL) {
		fprintf(stderr,"Cannot set root node of NULL graph\n");
		return;
	}
	graph->rootNodeIndex = root;
	return;
}

void dsgScenegraphRemoveNode(dsgScenegraph* graph, dsgNode* node) {
	if (graph == NULL) {
		fprintf(stderr,"Cannot remove node from NULL graph.\n");
		return;
	}

	if (node == NULL) {
		fprintf(stderr,"Cannot remove NULL node from graph.\n");
		return;
	}

	int i = dsgScenegraphGetIndexOfNode(graph, node);
	if (i < 0) {
		fprintf(stderr,"Cannot find node in graph to remove it.\n");
		return;
	}

	free(node);
	graph->nodes[i] = NULL;
	return;
}

void dsgScenegraphTraverseNodeVector(dsgScenegraph* graph, void (*function)(dsgNode*, void*), void* arg) {
	int i;
	dsgNode* next;
	for (i=0;i<NODES;i++) {
		next = graph->nodes[i];
		if (next == NULL) {
			continue;
		}
		(*function)((void*)next, arg == NULL ? graph : arg);
	}
	return;
}

dsgNode* dsgScenegraphGetNodeByName(dsgScenegraph* graph, char* name) {
	int i;
	dsgNode* next;
	for (i=0;i<NODES;i++) {
		next = graph->nodes[i];
		if (next == NULL) {
			continue;
		}
		if (next->name == NULL) {
			fprintf(stderr,"dsgScenegraphGetNodeByName: Cannot test node with NULL name\n");
			continue;
		}
		if (strcmp(graph->nodes[i]->name,name) == 0) {
			return graph->nodes[i];
		}
	}
	return NULL;
}

int dsgScenegraphTraversePath(dsgScenegraph* graph, char* path, void (*function)(dsgNode*, void*), void* arg) {
	char* start    = NULL;
	char* end      = NULL;
	int done       = 0;
	int nameLen    = 0;
	dsgNode* target = NULL;
	//fprintf(stdout,"Traversing path: %s\n",path);
	start = path+1;
	if (start == NULL) {
		fprintf(stderr,"Path is null\n");
		return done;
	}
	while (!done) {
		nameLen = 0;
		end = strchr(start+1, dsgScenegraphPathDelimeter);
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
		target = dsgScenegraphGetNodeByName(graph,tmp);
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

int dsgScenegraphCountChildrenOfNode(dsgScenegraph* graph, dsgNode* node) {
	int retval = 0;
	int i;
	dsgNode* next;
	for (i = 0; i<NODES; i++) {
		next = graph->nodes[i];
		if (next == NULL) {
			continue;
		}

		if (dsgScenegraphNodeIsChildOf(graph, node, next)) {
			retval++;
		}
	}
	return retval;
}

void dsgScenegraphUpdatePaths(dsgScenegraph* graph) {
	int i;
	dsgNode* next;
	for (i=0;i<NODES;i++) {
		next = graph->nodes[i];
		if (next != NULL) {
			dsgScenegraphGeneratePathForNode(graph, next);
		}
	}
	return;
}

void dsgScenegraphGeneratePathForNode(dsgScenegraph* graph, dsgNode* node) {
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
		if (!dsgScenegraphIsRootNode(graph,node)) {
			fprintf(stderr, "Cannot generate path for non-root node with no parent.\n");
			return;
		}
		fprintf(stdout,"Generating path for node root %s\n", node->name);
		memcpy(pathBuffer, &dsgScenegraphPathDelimeter, 1);
		pathBuffer[2] = '\0';
		strncat(pathBuffer, node->name, strlen(node->name));
	} else {
		dsgNode* parent = graph->nodes[node->parentIndex];
		if (parent == NULL) {
			fprintf(stderr,"Error finding node parent to create path\n");
			return;
		}
		fprintf(stdout,"Generating path for node %s with parent %s\n", node->name, parent->name);
		memcpy(pathBuffer, parent->path, strlen(parent->path)+1);
		strncat(pathBuffer, &dsgScenegraphPathDelimeter, strlen(&dsgScenegraphPathDelimeter));
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

int dsgScenegraphIsRootNode(dsgScenegraph* graph, dsgNode* node) {
	return graph->rootNodeIndex == dsgScenegraphGetIndexOfNode(graph,node);
}

int dsgScenegraphAddVertexBuffer(dsgScenegraph* graph, void* buffer) {
	fprintf(stdout,"Adding vertex buffer to graph\n");
	int index = dsgScenegraphGetNextAvailableVertexBuffer(graph);
	if (index < 0) {
		fprintf(stderr,"Unable to add vertex buffer, graph is full\n");
		return -1;
	}
	graph->vertexBuffers[index] = (float*)buffer;
	return index;
}

int  dsgScenegraphGetNextAvailableVertexBuffer(dsgScenegraph* graph) {
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

int  dsgScenegraphGetIndexOfVertexBuffer(dsgScenegraph* graph, void* buffer) {
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

int dsgScenegraphRemoveVertexBuffer(dsgScenegraph* graph, void* buffer) {
	if (graph == NULL || buffer == NULL) {
		return -1;
	}

	int index = dsgScenegraphGetIndexOfNode(graph,buffer);
	if (index < 0) {
		fprintf(stderr,"Cannot remove vertex buffer from graph %s, not found\n",
				                graph->name != NULL ? graph->name : "Unamed Graph");
		return -1;
	}

	free(graph->vertexBuffers[index]);
	graph->vertexBuffers[index] = NULL;
	return index;
}

int dsgScenegraphNodeIsParentOf (dsgScenegraph* graph, dsgNode* parent, dsgNode* child) {
	int retval = 0;
	if (parent < 0 || !dsgNodeHasValidName(child) || !dsgNodeHasValidPath(child)) {
		return retval;
	}
	retval = child->parentIndex == dsgScenegraphGetIndexOfNode(graph,parent);
	return retval;
}

int dsgScenegraphNodeIsChildOf(dsgScenegraph* graph, dsgNode* parent, dsgNode* child) {
	if (parent == NULL || child == NULL ||
	    !dsgNodeHasValidPath(child) ||
	    !dsgNodeHasValidName(child)) {
		return 0;
	}
	return child->parentIndex == dsgScenegraphGetIndexOfNode(graph,parent);
}

void dsgScenegraphNodeSetParent (dsgScenegraph* graph, dsgNode* parent, dsgNode* node) {
	dsgNodeSetParentIndex(dsgScenegraphGetIndexOfNode(graph,parent),node);
	return;
}

void dsgScenegraphPrintLine(dsgNode* node, void* arg) {
	fprintf(stdout,"%s\n",node->path);
	return;
}

void dsgScenegraphPrintGraph(dsgScenegraph* graph) {
	dsgScenegraphTraverseNodeVector(graph,&dsgScenegraphPrintLine,NULL);
	return;
}
