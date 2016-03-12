#ifndef DCW_DEFAULTCOLLISIONCONFIGURATION_H
#define DCW_DEFAULTCOLLISIONCONFIGURATION_H

struct 
dcwDefaultCollisionConfiguration;

typedef struct 
dcwDefaultCollisionConfiguration dcwDefaultCollisionConfiguration;

#ifdef __cplusplus
extern "C" {
#endif

dcwDefaultCollisionConfiguration* dcwDefaultCollisionConfigurationCreate(void);
void dcwDefaultCollisionConfigurationDestroy(dcwDefaultCollisionConfiguration*);

#ifdef __cplusplus
}
#endif

#endif // DCW_DEFAULTCOLLISIONCONFIGURATION_H
