#ifndef DCWCOLLISIONOBJECT_H
#define DCWCOLLISIONOBJECT_H

#include "dcwTransform.h"
#include "dcwBoxShape.h"

#ifndef __cplusplus
#define	ACTIVE_TAG             1
#define	ISLAND_SLEEPING        2
#define	WANTS_DEACTIVATION     3
#define	DISABLE_DEACTIVATION   4
#define	DISABLE_SIMULATION     5
#endif
struct dcwCollisionObject;
typedef struct dcwCollisionObject dcwCollisionObject;

#ifdef __cplusplus
extern "C" {
#endif
	dcwCollisionObject* dcwCollisionObjectCreate                 ();
	void                dcwCollisionObjectDestroy                (dcwCollisionObject*);
	void                dcwCollisionObjectSetCollisionBoxShape   (dcwCollisionObject*, dcwBoxShape*);
	void                dcwCollisionObjectSetWorldTransform      (dcwCollisionObject*, dcwTransform*);
	void                dcwCollisionObjectForceActivationState   (dcwCollisionObject*, int);
	void dcwCollisionObjectSetWorldTransform                     (dcwCollisionObject*, dcwTransform*);
#ifdef __cplusplus
}
#endif

#endif // DCWCOLLISIONOBJECT_H
