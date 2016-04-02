#ifndef TEST_DSGCAMERA_H
#define TEST_DSGCAMERA_H

#include "../../src/Unit/Unit.h"

namespace Dream {
	namespace Scene {
		namespace Scenegraph {
			namespace Test {
				class TestCamera : public Dream::Unit::Unit {
				public:
					TestCamera(void);
					~TestCamera(void);
					void run(void);
					void testCamera(void);
				};
			}
		}
	}
}

#endif  //TEST_DSGCAMERA_H
