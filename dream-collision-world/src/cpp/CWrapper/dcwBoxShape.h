#ifndef DCWBOXSHAPE_H
#define DCWBOXSHAPE_H

struct dcwBoxShape;
typedef struct dcwBoxShape dcwBoxShape;

#ifdef __cplusplus
extern "C" {
#endif
	dcwBoxShape* dcwBoxShapeCreate  ();
	void         dcwBoxShapeDestroy (dcwBoxShape*);

#ifdef __cplusplus
}
#endif

#endif  //DCWBOXSHAPE_H
