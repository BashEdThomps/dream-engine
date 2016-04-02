#ifndef TEST_NODE_H
#define TEST_NODE_H

#include "../../src/Unit/Unit.h"

namespace Dream {
	namespace Scene {
		namespace Test {
			class TestSceneObject : public Dream::Unit::Unit {
			public:
				TestSceneObject(void);
				~TestSceneObject(void);
				void run(void);
				void testRelationships(void);
				void testCanHoldName(void);
			};
		} // Test
	} // Scene
} // Dream

#endif //TEST_NODE_H
