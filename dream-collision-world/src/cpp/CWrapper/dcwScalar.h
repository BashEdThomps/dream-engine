#ifndef DCWSCALAR_H

struct dcwScalar;
typedef struct dcwScalar dcwScalar;

#ifdef __cplusplus
extern "C" {
#endif
	 dcwScalar* dcwScalarCreate (float);
	void dcwScalarDestroy (dcwScalar*);
	float dcwScalarGetValue(const dcwScalar*);

#ifdef __cplusplus
}
#endif

#define DCWSCALAR_H
#endif // DCWSCALAR_H
