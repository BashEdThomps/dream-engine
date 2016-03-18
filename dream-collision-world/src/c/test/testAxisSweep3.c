#include "testAxisSweep3.h"
#include "../../cpp/CWrapper/dcwAxisSweep3.h"
#include "../../../../unit/src/unit.h"

void testAxisSweep3(void) {
    unitTestHeading("AxisSweep3");
    testAxisSweep3Create();
    testAxisSweep3Destroy();
    unitTestFooter("AxisSweep3");
}

void testAxisSweep3Create(void) {
    dcwScalar* ten      = dcwScalarCreate (10.0f);
    dcwScalar* minusten = dcwScalarCreate (-10.0f);

    dcwVector3 *min, *max;
    min = dcwVector3Create (minusten,minusten,minusten);
    max = dcwVector3Create (ten,ten,ten);
    dcwAxisSweep3* as3 = dcwAxisSweep3Create(min,max);
    unitAssertNotNull("axissweep3create",as3);
}

void testAxisSweep3Destroy(void) {
    dcwScalar* ten      = dcwScalarCreate(10.0f);
    dcwScalar* minusten = dcwScalarCreate(-10.0f);
    dcwVector3 *min, *max;

    min = dcwVector3Create (minusten,minusten,minusten);
    max = dcwVector3Create (ten,ten,ten);

    dcwAxisSweep3* as3 = dcwAxisSweep3Create(min,max);
    unitAssertNotNull("AxisSweep3Create",as3);

    dcwAxisSweep3Destroy(as3);
    as3 = 0;
    unitAssertNull("AxisSweep3Destroyed",as3);
}
