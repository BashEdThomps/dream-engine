#ifndef TEST_NODE_H
#define TEST_NODE_H

#include "../../dream-unit/src/Unit.h"

namespace DreamScenegraphTest {
	class TestNode : public DreamUnit::Unit {
	public:
		TestNode::TestNode(void);
		TestNode::~TestNode(void);
		void run(void);
		void testNode(void);
	};
}

#endif //TEST_NODE_H
