#include <unit.h>

#include "../../cpp/CWrapper/dcwScalar.h"
#include "../../cpp/CWrapper/dcwVector3.h"
#include "../../cpp/CWrapper/dcwBoxShape.h"

#include "testBoxShape.h"

void testBoxShape(void) {
	unitPrintComment("Box Shape");
	dcwScalar *x, *y, *z;
	x = dcwScalarCreate  (1.0f);
	y = dcwScalarCreate (2.0f);
	z = dcwScalarCreate (4.0f);

	dcwVector3* boxSize = dcwVector3Create(x,y,z);
	unitAssertNotNull("Size Vector for BoxShape",boxSize);

	dcwBoxShape* shape = dcwBoxShapeCreate(boxSize);
	unitAssertNotNull("Box Shape Object", shape);

	// TODO - Test  box shape params equal to boxSize

	//unitAssertFail("TestBoxShape");
	return;
}
