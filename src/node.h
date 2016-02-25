#ifndef NODE_H
#define NODE_H
/*
* node.h
*/
typedef struct {
	char* name;
	struct node_t* parent;
	int numChildren;
	struct node_t* children;
} node_t;

void dsNodeInit(node_t* node);
void dsNodeInitWithName(node_t* node, char* name);
void dsNodeAddChild   (node_t* parent, node_t* child);
void dsNodeRemoveChild(node_t* parent, node_t* child);
void dsNodePrintGraph (node_t* node);
int dsNodeIsChildOf  (node_t* parent, node_t* child);
int dsNodeIsParentOf (node_t* parent, node_t* child);

#endif // NODE_H
