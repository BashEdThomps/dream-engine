#ifndef TEST_DSGCAMERA_H
#define TEST_DSGCAMERA_H

#include "../../dream-unit/src/Unit.h"

namespace DreamScenegraphTest {
	class TestCamera : public Dream::Unit::Unit {
	public:
		TestCamera(void);
		~TestCamera(void);
		void run(void);
		void testCamera(void);
	};
}

#endif  //TEST_DSGCAMERA_H
