#ifndef DCWVECTOR3_H
#define DCWVECTOR3_H

struct dcwVector3;
typedef struct dcwVector3 dcwVector3;

#ifdef __cplusplus
extern "C" {
#endif
	dcwVector3* dcwVector3Create  (float, float, float);
	void        dcwVector3Destroy (dcwVector3*);
#ifdef __cplusplus
}
#endif

#endif  // DCWVECTOR3_H
