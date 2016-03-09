#include "../../../unit/src/unit.h"
#include "testVector3.h"
#include "../../cpp/CWrapper/dcwScalar.h"
#include "../../cpp/CWrapper/dcwVector3.h"

void testVector3(void) {
	unitPrintComment("Vector3");
	dcwScalar* expectedX = dcwScalarCreate(1.0f);
	dcwScalar* expectedY = dcwScalarCreate(2.0f);
	dcwScalar* expectedZ = dcwScalarCreate(4.0f);

	 dcwVector3* vec = dcwVector3Create(expectedX,expectedY,expectedZ);

	const dcwScalar* actualX = dcwVector3GetX(vec);
	const dcwScalar* actualY = dcwVector3GetY(vec);
	const dcwScalar* actualZ = dcwVector3GetZ(vec);

	unitAssertNotZero("Vector3 X", dcwScalarGetValue(expectedX) ==
			               dcwScalarGetValue(actualX));

	unitAssertNotZero("Vector3 Y", dcwScalarGetValue(expectedY) ==
			               dcwScalarGetValue(actualY));

	unitAssertNotZero("Vector3 Z", dcwScalarGetValue(expectedZ) ==
			               dcwScalarGetValue(actualZ));
	return;
}
