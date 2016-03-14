#ifndef DCW_DISPATCHER_H
#define DCW_DISPATCHER_H

struct dcwCollisionDispatcher;
typedef struct dcwCollisionDispatcher dcwCollisionDispatcher;

#include "dcwDefaultCollisionConfiguraion.h"
#include "dcwPersistentManifold.h"

#ifdef __cplusplus
extern "C" {
#endif
	dcwCollisionDispatcher*     dcwCollisionDispatcherCreate  (dcwDefaultCollisionConfiguration*);
	void                        dcwCollisionDispatcherDestroy (dcwCollisionDispatcher*);
	int                         dcwCollisionDispatcherGetNumManifolds(dcwCollisionDispatcher*);
	dcwPersistentManifold*      dcwGetManifoldByIndexInternal(dcwCollisionDispatcher* , int);
#ifdef __cplusplus
}
#endif

#endif // DCW_DISPATCHER_H
