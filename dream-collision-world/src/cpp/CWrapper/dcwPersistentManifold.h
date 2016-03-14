#ifndef DCW_PERSISTENEMANIFOLD_H
#define DCW_PERSISTENEMANIFOLD_H

#include "dcwCollisionObject.h"
#include "dcwManifoldPoint.h"
#include "dcwPersistentManifold.h"

struct dcwPersistentManifold;
typedef struct dcwPersistentManifold dcwPersistentManifold;

#ifdef __cplusplus
extern "C" {
#endif

dcwPersistentManifold* dcwPersistentManifoldCreate();
void dcwPersistentManifoldDestroy(dcwPersistentManifold*);

const dcwCollisionObject* dcwPersistentManifoldGetBody0(dcwPersistentManifold*);
const dcwCollisionObject* dcwPersistentManifoldGetBody1(dcwPersistentManifold*);
int dcwPersistentManifoldGetNumContacts(dcwPersistentManifold*);
dcwManifoldPoint* dcwPersistentManifoldGetContactPoint(dcwPersistentManifold*, int);

	//const dcwVector3* dcwManifoldPointGetNormalWorldOnA   (dcwManifoldPoint*);
#ifdef __cplusplus
}
#endif

#endif // DCW_PERSISTENEMANIFOLD_H
