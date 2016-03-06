#include <unit.h>
#include "../dsgCamera.h"
#include "testCamera.h"

void testCamera(void) {
    unitTestHeading("DreamSceneGraph Camera");

    dsgCamera *camera = dsgCameraInit();

    float laX, laY, laZ;
    laX =  22.0f;
    laY = -34.0f;
    laZ =  0.0f;

    dsgCameraSetLookAt(camera, laX,laY,laZ);

    unitAssertEqualFloat("Camera LookAt X", camera->lookAt[DSG_CAM_X], laX);
    unitAssertEqualFloat("Camera LookAt Y", camera->lookAt[DSG_CAM_Y], laY);
    unitAssertEqualFloat("Camera LookAt Z", camera->lookAt[DSG_CAM_Z], laZ);

    float trX, trY, trZ;
    trX =  10.0f;
    trY = -10.0f;
    trZ =  100.0f;

    dsgCameraSetTranslation(camera, trX,trY,trZ);

    unitAssertEqualFloat("Camera Translation X", camera->translation[DSG_CAM_X], trX);
    unitAssertEqualFloat("Camera Translation Y", camera->translation[DSG_CAM_Y], trY);
    unitAssertEqualFloat("Camera Translation Z", camera->translation[DSG_CAM_Z], trZ);

    float upX, upY, upZ;

    upX = -8.0f;
    upY =  16.0f;
    upZ = -32.0f;

    dsgCameraSetUp(camera, upX,upY,upZ);

    unitAssertEqualFloat("Camera Up X", camera->up[DSG_CAM_X], upX);
    unitAssertEqualFloat("Camera Up Y", camera->up[DSG_CAM_Y], upY);
    unitAssertEqualFloat("Camera Up Z", camera->up[DSG_CAM_Z], upZ);

    dsgCameraDestroy(camera);

    unitTestFooter("DreamSceneGraph Camera");
}
