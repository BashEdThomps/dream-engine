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
}

void dsgGraphSetRootNodeIndex(graph_t* graph, int root) {
	graph->rootNodeIndex = root;
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

void dsgGraphUpdateOrder(graph_t* graph) {
	dsgGraphInitOrderedNodes(graph);
	node_t* node = dsgGraphGetRootNode(graph); 

	if (graph == NULL) {
		fprintf(stderr,"Error: Cannot update order on NULL graph!\n");
		return;
	}

       	if (node == NULL ) {
		fprintf(stderr,"Error: Cannot update order on graph from NULL node\n");
		return;	
	}

	fprintf(stdout,"Updating Graph Order of %s\n", graph->name ? graph->name : "Untitled Graph");
	
	dsgGraphUpdateOrderFromNode(graph,node);
	return;
}

void dsgGraphUpdateOrderFromNode(graph_t* graph, node_t* node) {
	fprintf(stdout,"dsgGUOFN On node %s\n", node->name);
	int children =  node->numImmediateChildren; 
	int index = dsgGraphGetNextAvailableOrderedNodeIndex(graph);

	if (index < 0) {
		fprintf(stderr,"Unable to find an available ordered node index\n");
		return;
	}

	graph->orderedNodes[index] = dsgGraphGetIndexOfNode(graph,node);

	if (children > 0) {
		int i;
		for (i=0;i<children;i++) {
			dsgGraphUpdateOrderFromNode(graph, graph->nodes[node->immediateChildren[i]]);	
		}
	}
	return;
}

int dsgGraphGetNextAvailableOrderedNodeIndex(graph_t* graph) {
	int i;
	int index;
	for(i=0; i < NODES; i++) {
		index = NODES-i-1;
		fprintf(stdout,"dsgGGNAODI %d\n",index);
		if(graph->orderedNodes[index] == -1) {
			fprintf(stdout,"dsgGGNAODI found\n");
			return index;	
		}	
	}
	fprintf(stdout,"dsgGGNAODI not found\n");
	return -1;
}

void dsgGraphTraverseOrderedNodes(graph_t* graph, void (*function)(node_t*, void*), void* arg) {
	int i;
	node_t* next;
	for (i=NODES-1;i>=0;i--) {
		if (graph->orderedNodes[i] == -1) {
			break;
		}
		next = graph->nodes[graph->orderedNodes[i]];
		if (next==NULL) {
			continue;
		}	
		(*function)((void*)next, arg == NULL ? graph : arg);
	}
	return;
}

void dsgGraphPrintGraph(graph_t* graph) {
	dsgGraphTraverseOrderedNodes(graph,&dsgNodePrint,NULL);
	return;
}

void dsgGraphUpdateTranslationRotation(graph_t* graph) {
	dsgNodeSumWithParentArg_t* arg = (dsgNodeSumWithParentArg_t*)malloc(sizeof(dsgNodeSumWithParentArg_t));
	int i;
	node_t* next;
	for (i=NODES-1;i>=0;i--) {
		int nextIndex = graph->orderedNodes[i];
		if (nextIndex < 0) {
			continue;
		}
		next = graph->nodes[nextIndex];
		if (next == NULL) {
			continue;
		}	
		int parentIndex = next->parentIndex;
		arg->node = next;
		arg->parent = NULL;
		if (parentIndex > -1 ) {
			fprintf(stdout,"ParentIndex: %d\n",parentIndex);
			arg->parent = graph->nodes[parentIndex];
		}
		(*dsgNodeSumWithParentTranslationRotation)((void*)arg);
	}
	free(arg);
	return;
}

void dsgGraphUpdateChildren(graph_t* graph) {
	int i;
	node_t* node;
	for (i=0; i<NODES; i++) {
		node = graph->nodes[i];	

		if (node == NULL) {
			continue;
		}

		if (node->children != NULL) {
			fprintf(stderr,"Freeing node's existing children vector\n");
			free(node->children);
		}

		fprintf(stderr,"Counting children of node %s\n", node->name);
		int numChildren = dsgGraphCountChildrenOfNode(graph, node);
		node->numChildren = numChildren;
		fprintf(stdout,"Found %d\n",numChildren);
		if (numChildren > 0) {
			fprintf(stderr,"Allocating memory for children\n");
			node->children = (int*)malloc(sizeof(int)*numChildren);
			dsgGraphPopulateChildren(graph, node);
		} else {
			fprintf(stdout,"No children found for node %s\n",node->name);
		}
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

		if (node->immediateChildren != NULL) {
			fprintf(stderr,"Freeing node's existing immediate children vector\n");
			free(node->immediateChildren);
		}

		fprintf(stderr,"Counting immediate children of node %s\n", node->name);
		int numImmediateChildren = dsgGraphCountImmediateChildrenOfNode(graph, node);
		node->numImmediateChildren = numImmediateChildren;
		fprintf(stdout,"Found %d\n",numImmediateChildren);
		if (numImmediateChildren > 0) {
			fprintf(stderr,"Allocating memory for Immediate children\n");
			node->immediateChildren = (int*) malloc(sizeof(int)*numImmediateChildren);
			dsgGraphPopulateImmediateChildren(graph, node);
		} else {
			fprintf(stderr,"No immediate children found for %s\n",node->name);
		}
	}
	return;
}

void dsgGraphPopulateChildren(graph_t* graph, node_t* node) {
	node_t* next;
	int i;
	int childrenFound = 0;
	for (i=0; i < NODES; i++) {
		next = graph->nodes[i];
		if (next == NULL) {
			continue;
		}
		if (dsgGraphNodeIsChildOf(graph,node, next)) {
			fprintf(stdout,"PopulateChildren: child found for %s\n",node->name);
			int* childrenArray = (int*)node->children;
			childrenArray[childrenFound] = i;	
			childrenFound++;
		}				
	} 
}

void dsgGraphPopulateImmediateChildren(graph_t* graph, node_t* node) {
	node_t* next;
	int i;
	int iChildrenFound = 0;
	for (i=0; i < NODES; i++) {
		next = graph->nodes[i];
		if (next == NULL) {
			continue;
		}
		if (dsgGraphNodeIsImmediateChildOf(graph, node, next)) {
			int* iChildrenArray = (int*)node->immediateChildren;
			iChildrenArray[iChildrenFound] = i;	
			iChildrenFound++;
		}				
	} 
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

int dsgGraphCountImmediateChildrenOfNode(graph_t* graph, node_t* node) {
	int retval = 0;
	int i;
	for (i=0;i<NODES;i++) {
		node_t* next = graph->nodes[i];
		if (dsgGraphNodeIsImmediateChildOf(graph, node, next)) {
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
		memcpy(pathBuffer, "/\0", 2);	
		strncat(pathBuffer, node->name, strlen(node->name));
	} else {
		node_t* parent = graph->nodes[node->parentIndex];
		if (parent == NULL) {
			fprintf(stderr,"Error finding node parent to create path\n");
			return;
		}
		fprintf(stdout,"Generating path for node %s with parent %s\n", node->name, parent->name);
		memcpy(pathBuffer, parent->path, strlen(parent->path)+1);
		strncat(pathBuffer, "/\0", 2);	
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

void dsgGraphUpdateAll(graph_t* graph) {
	dsgGraphUpdatePaths    (graph);
	dsgGraphUpdateChildren (graph);
	dsgGraphUpdateImmediateChildren(graph);
	dsgGraphUpdateOrder(graph);
	dsgGraphUpdateTranslationRotation(graph);
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

void dsgGraphInitOrderedNodes(graph_t* graph) {
	int i;
	for (i=0; i<NODES; i++) {
		graph->orderedNodes[i] = -1;	
	}
	return;
}

/* 
 * Node Relationships 
 */
int dsgGraphNodeIsParentOf (graph_t* graph ,node_t* parent, node_t* child) {
	int retval = 0; 

	if (!dsgNodeHasValidPath(child) || !dsgNodeHasValidPath(parent) ||
	    !dsgNodeHasValidName(child) || !dsgNodeHasValidName(parent)) {
		return retval;	
	}

	char* indexOfParent = strstr(child->path,parent->name); 
	char* indexOfChild = strstr(child->path,child->name);

	if (indexOfParent != NULL && indexOfChild != NULL) {
		retval = indexOfChild > indexOfParent;
	}

	return retval;
}

int dsgGraphNodeIsImmediateParentOf (graph_t* graph, node_t* parent, node_t* child) {
	int retval = 0; 

	if (parent < 0 || !dsgNodeHasValidName(child) || !dsgNodeHasValidPath(child)) {
		return retval;	
	}

	retval = child->parentIndex == dsgGraphGetIndexOfNode(graph,parent);
	return retval;
}

int dsgGraphNodeIsChildOf(graph_t* graph, node_t* parent, node_t* child) {
	int retval = 0; 

	if (graph == NULL || parent == NULL || child == NULL || 
	    !dsgNodeHasValidPath(parent) || !dsgNodeHasValidName(parent) ||
	    !dsgNodeHasValidPath(child) || !dsgNodeHasValidName(child) ) {
		return retval;	
	}
	char* indexOfParent = strstr(child->path,parent->name);
	char* indexOfChild = strstr(child->path,child->name);

	if (indexOfParent != NULL && indexOfChild != NULL) {
		retval = indexOfChild > indexOfParent;
	}

	return retval;
}

int dsgGraphNodeIsImmediateChildOf(graph_t* graph, node_t* parent, node_t* child) {
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
