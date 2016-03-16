#ifndef DCW_AXISSWEEP3_H
#define DCW_AXISSWEEP3_H

#include "dcwVector3.h"

struct dcwAxisSweep3;
typedef struct dcwAxisSweep3 dcwAxisSweep3;

#ifdef __cplusplus
#include "../BulletCollision/BroadphaseCollision/btAxisSweep3.h"
extern "C" {
#endif

dcwAxisSweep3* dcwAxisSweep3Create  (dcwVector3*, dcwVector3*);
void           dcwAxisSweep3Destroy (dcwAxisSweep3*);

#ifdef __cplusplus
}
#endif

#endif  //DCW_AXISSWEEP3_H
