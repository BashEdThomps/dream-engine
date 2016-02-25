/*
* node.h
*/
typedef struct node_t;

typedef struct {
	node_t* parent;
} node_t;

void dsNodeAddChild   (node_t* parent, node_t* child);
void dsNodeRemoveChild(node_t* parent, node_t* child);
void dsNodePrintGraph (node_t* node);
void dsNodeIsChildOf  (node_t* parent, node_t* child );
void dsNodeIsParentOf (node_t* parent, node_t* child);

