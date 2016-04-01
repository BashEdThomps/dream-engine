#ifndef TEST_NODE_H
#define TEST_NODE_H

#include "../../dream-unit/src/Unit.h"

namespace DreamScenegraphTest {
	class TestSceneObject : public Dream::Unit::Unit {
	public:
		TestSceneObject::TestSceneObject(void);
		TestSceneObject::~TestSceneObject(void);
		void run(void);
		void testSceneObject(void);
	};
}

#endif //TEST_NODE_H
