#ifndef TEST_GRAPH_H
#define TEST_GRAPH_H

#include "../../src/Unit/Unit.h"

namespace Dream {
	namespace Scene {
		namespace Test {
			class TestScenegraph : public Dream::Unit::Unit {
			public:
				TestScenegraph                        (void);
				~TestScenegraph                       (void);
				void run                              (void);
				void testCanStoreMultipleSceneObjects (void);
				void testCanEstablishRelationships    (void);
				void testCanGetSceneObjectByName      (void);
			};
		}
	}
}
#endif // TEST_GRAPH_H
