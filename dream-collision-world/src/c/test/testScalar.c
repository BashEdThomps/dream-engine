#include "../../../../unit/src/unit.h"

#include "../../cpp/CWrapper/dcwScalar.h"
#include "testScalar.h"

void testScalar(void) {
	unitPrintComment("Scalar");
	dcwScalar *test1 = dcwScalarCreate(20.5f);
	dcwScalar *test2 = dcwScalarCreate(40.1f);
	dcwScalar *test3 = dcwScalarCreate(50.5f);

	unitAssertEqualFloat("Test Scalar equality 1", dcwScalarGetValue(test1),   20.5f);
	unitAssertEqualFloat("Test Scalar equality 2", dcwScalarGetValue(test2),   40.1f);
	unitAssertNotEqualFloat("Test Scalar equality 3",dcwScalarGetValue(test3),  0.0f);

	return;
}
