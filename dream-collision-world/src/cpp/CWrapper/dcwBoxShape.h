#ifndef DCWBOXSHAPE_H
#define DCWBOXSHAPE_H

#include "dcwVector3.h"

struct dcwBoxShape;
typedef struct dcwBoxShape dcwBoxShape;

#ifdef __cplusplus
extern "C" {
#endif
	dcwBoxShape* dcwBoxShapeCreate  (dcwVector3*);
	void         dcwBoxShapeDestroy (dcwBoxShape*);

#ifdef __cplusplus
}
#endif

#endif  //DCWBOXSHAPE_H
