#include "Dream.h"

namespace Dream {
	Dream::Dream() {
	}

	Dream::~Dream() {
	}

	Scene* Dream::getScene() {
		return mScene;
	}

	void Dream::setScene(Scene* scene) {
		mScene = scene;
	}
}
