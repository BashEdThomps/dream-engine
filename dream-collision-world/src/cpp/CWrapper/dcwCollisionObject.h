#ifndef DCWCOLLISIONOBJECT_H
#define DCWCOLLISIONOBJECT_H

#include "dcwBoxShape.h"

struct dcwCollisionObject;
typedef struct dcwCollisionObject dcwCollisionObject;

#ifdef __cplusplus
extern "C" {
#endif
	dcwCollisionObject* dcwCollisionObjectCreate                 ();
	void                dcwCollisionObjectDestroy                (dcwCollisionObject*);
	void                dcwCollisionObjectSetCollisionBoxShape   (dcwCollisionObject*, dcwBoxShape*);
#ifdef __cplusplus
}
#endif

#endif // DCWCOLLISIONOBJECT_H
