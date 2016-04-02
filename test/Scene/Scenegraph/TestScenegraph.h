#ifndef TEST_GRAPH_H
#define TEST_GRAPH_H

#include "../../../src/Unit/Unit.h"

namespace Dream {
	namespace Scene {
		namespace Scenegraph {
			namespace Test {
				class TestScenegraph : public Dream::Unit::Unit {
					TestScenegraph::TestScenegraph(void);
					TestScenegraph::~TestScenegraph(void);
					void run(void);
					void testScenegraph(void);
				};
			}
		}
	}
}
#endif // TEST_GRAPH_H
