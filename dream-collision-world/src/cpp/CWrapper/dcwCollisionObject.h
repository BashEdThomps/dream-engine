#ifndef DCWCOLLISIONOBJECT_H
#define DCWCOLLISIONOBJECT_H

struct dcwCollisionObject;
typedef struct dcwCollisionObject dcwCollisionObject;

#ifdef __cplusplus
extern "C" {
#endif
	dcwCollisionObject* dcwCollisionObjectCreate  ();
	void                dcwCollisionObjectDestroy (dcwCollisionObject*);

#ifdef __cplusplus
}
#endif

#endif // DCWCOLLISIONOBJECT_H
