#include "../../../unit/src/unit.h"

#include "../../cpp/CWrapper/dcwScalar.h"
#include "../../cpp/CWrapper/dcwBoxShape.h"
#include "../../cpp/CWrapper/dcwCollisionObject.h"

#include "testCollisionObject.h"

void testCollisionObject(void) {
	unitPrintComment("Collision Object");
	
	dcwScalar *x, *y, *z;
	x = dcwScalarCreate  (2.0f);
	y = dcwScalarCreate  (4.0f);
	z = dcwScalarCreate  (8.0f);

	dcwVector3* boxSize = dcwVector3Create(x,y,z);
	unitAssertNotNull("Collision Object Size Vector for BoxShape",boxSize);

	dcwBoxShape* shape = dcwBoxShapeCreate(boxSize);
	unitAssertNotNull("Collision Object Box Shape Object", shape);

	dcwCollisionObject* colObj = dcwCollisionObjectCreate();
	dcwCollisionObjectSetCollisionBoxShape(colObj,shape);

	unitAssertNotNull("CollisionObject Exists",colObj);

	return;
}
