#include <unit.h>

#include "../../cpp/CWrapper/dcwScalar.h"
#include "testScalar.h"

void testScalar(void) {
	unitPrintComment("Scalar");
	unitAssertFail("dcwScalar");
	return;
}
