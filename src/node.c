#include <stdio.h>
#include "node.h"


void dsNodeInit(node_t* node) {
	node->numChildren = 0;
	node->children = 0;
}

void dsNodeInitWithName(node_t* node, char* name) {
	node->name = name;
	dsNodeInit(node);
}


void dsNodeAddChild(node_t* parent, node_t* child) {
	if (parent == 0) {
		fprintf(stderr,"dsNodeAddChild: Parent cannot be NULL\n");
		return;	
	}
	if (child == 0) {
		fprintf(stderr,"dsNodeAddChild: Child cannot be NULL\n");
		return;	
	}
	if (parent->numChildren == 0) {
		if (parent->children == 0)	{
			parent->children = (struct node_t*)child;
			parent->numChildren = 1;
		}
	} else {
	
	}
	return;
}

void dsNodePrintGraph(node_t* node) {
	return;
}


int dsNodeIsChildOf  (node_t* parent, node_t* child ) {
	return 0;
}

int dsNodeIsParentOf (node_t* parent, node_t* child) {
	return 0;
}

