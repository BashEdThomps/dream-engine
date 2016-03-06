#include "dcwScalar.h"


#ifdef __cplusplus

#include "../LinearMath/btScalar.h"

extern "C" {
#endif
	 dcwScalar* dcwScalarCreate (float value) {
		return reinterpret_cast< dcwScalar*>(
			new btScalar(value)
		);
	}

	void dcwScalarDestroy ( dcwScalar* obj) {
		delete reinterpret_cast< btScalar*>(obj);
		return;
	}

	float dcwScalarGetValue( dcwScalar* scalar) {
		 btScalar* btSc = reinterpret_cast< btScalar*>(scalar);
		return (float)*btSc;
	}

#ifdef __cplusplus
}
#endif

