#include "../src/Camera.h"
#include "TestCamera.h"

namespace DreamScenegraphTest {

	TestCamera::TestCamera() {
	}

	TestCamera::~TestCamera() {
	}

	void CameraTest::run() {
		testCamera();
	}

	void CameraTest::testCamera(void) {
		testHeading("DreamSceneGraph Camera");

		DreamScenegraph::Camera *camera = new DreamScenegraph::Camera();

		float laX, laY, laZ;
		laX =  22.0f;
		laY = -34.0f;
		laZ =  0.0f;

		camera->setLookAt(camera, laX,laY,laZ);

		assertEqualFloat("Camera LookAt X", camera->lookAt[DSG_CAM_X], laX);
		assertEqualFloat("Camera LookAt Y", camera->lookAt[DSG_CAM_Y], laY);
		assertEqualFloat("Camera LookAt Z", camera->lookAt[DSG_CAM_Z], laZ);

		float trX, trY, trZ;
		trX =  10.0f;
		trY = -10.0f;
		trZ =  100.0f;

		camera->setTranslation(camera, trX,trY,trZ);

		assertEqualFloat("Camera Translation X", camera->getTranslation()[DSG_CAM_X], trX);
		assertEqualFloat("Camera Translation Y", camera->getTranslation()[DSG_CAM_Y], trY);
		assertEqualFloat("Camera Translation Z", camera->getTranslation()[DSG_CAM_Z], trZ);

		float upX, upY, upZ;

		upX = -8.0f;
		upY =  16.0f;
		upZ = -32.0f;

		camera->setUp(upX,upY,upZ);

		assertEqualFloat("Camera Up X", camera->up[DSG_CAM_X], upX);
		assertEqualFloat("Camera Up Y", camera->up[DSG_CAM_Y], upY);
		assertEqualFloat("Camera Up Z", camera->up[DSG_CAM_Z], upZ);

		delete camera;

		testFooter("DreamSceneGraph Camera");
	}
}
