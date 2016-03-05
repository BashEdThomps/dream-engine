#include "../../cpp/LinearMath/btVector3.h"

#include "dcwVector3.h"

#ifdef __cplusplus
extern "C" {
#endif

dcwVector3* dcwVector3Create  (float x, float y, float z) {
	return reinterpret_cast<dcwVector3*>(
		new btVector3(x,y,z)
	);
}

void dcwVector3Destroy (dcwVector3* vector) {
	delete reinterpret_cast<btVector3*>(vector);
	return;
}

#ifdef __cplusplus
}
#endif


