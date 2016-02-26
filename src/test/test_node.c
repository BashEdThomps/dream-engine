#include <unit.h>
#include <stdlib.h>
#include <string.h>

#include "test_node.h"
#include "../node.h"

void testNode() {
	unitTestHeading("Node");

	node_t* root = (node_t*)malloc(sizeof(*root));
	node_t* child1 = (node_t*)malloc(sizeof(*child1));
	node_t* child2 = (node_t*)malloc(sizeof(*child2));
	node_t* child1_1 = (node_t*)malloc(sizeof(*child1_1));
	node_t* child1_1_1 = (node_t*)malloc(sizeof(*child1_1_1));

	dsgNodeInit(root,"Root");
	unitAssertNull("Root Node Children", root->children);
	unitAssertZero("Root Node Init Num Children", root->numChildren);
	unitAssertZero("Root node name is Root",strcmp(root->name,"Root"));

	dsgNodeInit(child1,"Child1");
	unitAssertNull("Child1 Node Children", child1->children);
	unitAssertZero("Child1 Node Init Num Children", child1->numChildren);
	unitAssertZero("Child1 name is Child1",strcmp(child1->name,"Child1"));

	dsgNodeInit(child2,"Child2");
	unitAssertNull("Child2 Node Children", child2->children);
	unitAssertZero("Child2 Node Init Num Children", child2->numChildren);
	unitAssertZero("Child2 name is Child2",strcmp(child2->name,"Child2"));

	dsgNodeInit(child1_1,"Child1_1");
	unitAssertNull("Child1_1 Node Children", child1_1->children);
	unitAssertZero("Child1_1 Node Init Num Children", child1_1->numChildren);
	unitAssertZero("Child1_1 name is Child1_1",strcmp(child1_1->name,"Child1_1"));

	dsgNodeInit(child1_1_1,"Child1_1_1");
	unitAssertNull("Child1_1_1 Node Children", child1_1_1->children);
	unitAssertZero("Child1_1_1 Node Init Num Children", child1_1_1->numChildren);
	unitAssertZero("Child1_1_1 name is Child1_1_1",strcmp(child1_1_1->name,"Child1_1_1"));

	unitTestFooter("Node");
}
