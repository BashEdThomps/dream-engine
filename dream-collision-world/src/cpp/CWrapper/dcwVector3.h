#ifndef DCWVECTOR3_H
#define DCWVECTOR3_H

#include "dcwScalar.h"

struct dcwVector3;
typedef struct dcwVector3 dcwVector3;

#ifdef __cplusplus
extern "C" {
#endif
	 dcwVector3* dcwVector3Create  (
		 dcwScalar*,
		 dcwScalar*,
		 dcwScalar*
	);
	void dcwVector3Destroy ( dcwVector3*);

	const dcwScalar* dcwVector3GetX ( dcwVector3*);
	const dcwScalar* dcwVector3GetY ( dcwVector3*);
	const dcwScalar* dcwVector3GetZ ( dcwVector3*);

	void dcwVector3SetX ( dcwVector3*,  dcwScalar*);
	void dcwVector3SetY ( dcwVector3*,  dcwScalar*);
	void dcwVector3SetZ ( dcwVector3*,  dcwScalar*);

#ifdef __cplusplus
}
#endif

#endif  // DCWVECTOR3_H
