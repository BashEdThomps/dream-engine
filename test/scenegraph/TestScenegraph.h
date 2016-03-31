#ifndef TEST_GRAPH_H
#define TEST_GRAPH_H

#include "../../dream-unit/src/Unit.h"

namespace DreamScenegraphTest {
	class TestScenegraph : public DreamUnit::Unit {
		TestScenegraph::TestScenegraph(void);
		TestScenegraph::~TestScenegraph(void);
		void run(void);
		void testScenegraph(void);
		void traverseFunction(void);
	};
}	
void testGraph(void);

#endif // TEST_GRAPH_H
