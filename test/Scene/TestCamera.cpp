#include "../../src/Scene/Camera.h"
#include "TestCamera.h"

namespace Dream {
	namespace Scene {
		namespace Test {

			TestCamera::TestCamera() : Dream::Unit::Unit("Dream::Scene::Camera") {
			}

			TestCamera::~TestCamera() {
			}

			void TestCamera::run() {
				header();
				testLookAt();
				testUp();
				testTranslation();
			}

			void TestCamera::testLookAt(void) {
				Dream::Scene::Camera *camera = new Dream::Scene::Camera();
				float laX, laY, laZ;
				laX =  22.0f;
				laY = -34.0f;
				laZ =  0.0f;
				camera->setLookAt(laX,laY,laZ);
				assertTrue("Camera LookAt X", camera->getLookAt()[0] == laX);
				assertTrue("Camera LookAt Y", camera->getLookAt()[1] == laY);
				assertTrue("Camera LookAt Z", camera->getLookAt()[2] == laZ);
				delete camera;

			}

			void TestCamera::testTranslation(void) {
				Dream::Scene::Camera *camera = new Dream::Scene::Camera();
				float trX, trY, trZ;
				trX =  10.0f;
				trY = -10.0f;
				trZ =  100.0f;
				camera->setTranslation(trX,trY,trZ);
				assertTrue("Camera Translation X", camera->getTranslation()[0] == trX);
				assertTrue("Camera Translation Y", camera->getTranslation()[1] == trY);
				assertTrue("Camera Translation Z", camera->getTranslation()[2] == trZ);
				delete camera;
			}

			void TestCamera::testUp(void) {
				Dream::Scene::Camera *camera = new Dream::Scene::Camera();
				float upX, upY, upZ;
				upX = -8.0f;
				upY =  16.0f;
				upZ = -32.0f;
				camera->setUp(upX,upY,upZ);
				assertTrue("Camera Up X", camera->getUp()[0] == upX);
				assertTrue("Camera Up Y", camera->getUp()[1] == upY);
				assertTrue("Camera Up Z", camera->getUp()[2] == upZ);
				delete camera;
			}
		}
	}
}
