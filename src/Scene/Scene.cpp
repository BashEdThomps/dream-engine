/*
* Scene
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Scene.h"

namespace Dream {
	namespace  Scene {
		Scene::Scene() {
			init();
			mPhysicsEnabled   = false;
			mAnimationEnabled = false;
			mInputEnabled     = false;
			mAudioEnabled     = false;
		}

		Scene::Scene(nlohmann::json jsonScene) {
			init();
			mUUID             = jsonScene [SCENE_JSON_UUID];
			mName             = jsonScene [SCENE_JSON_NAME];

			mPhysicsEnabled = (
				jsonScene [SCENE_JSON_PHYSICS_ENABLED].is_null() ?
				false : (bool)jsonScene [SCENE_JSON_PHYSICS_ENABLED]
			);

			mAnimationEnabled = (
				jsonScene [SCENE_JSON_ANIMATION_ENABLED].is_null() ?
				false : (bool)jsonScene [SCENE_JSON_ANIMATION_ENABLED]
			);

			mInputEnabled = (
				jsonScene [SCENE_JSON_INPUT_ENABLED].is_null() ?
				false :(bool)jsonScene [SCENE_JSON_INPUT_ENABLED]
			);

			mAudioEnabled = (
				jsonScene [SCENE_JSON_AUDIO_ENABLED].is_null() ?
				false : (bool)jsonScene [SCENE_JSON_AUDIO_ENABLED]
			);
		}

		Scene::~Scene() {

		}

		void Scene::init() {
			mCamera = new Camera();
			mScenegraph = new Scenegraph();
		}

		std::string Scene::getName() {
			return mName;
		}

		std::string Scene::getUUID() {
			return mUUID;
		}

		void Scene::setUUID(std::string uuid) {
			mUUID = uuid;
		}

		void Scene::setName(std::string name) {
			mName = name;
		}

		Scenegraph* Scene::getScenegraph() {
			return mScenegraph;
		}

		bool Scene::isAudioEnabled() {
			return mAudioEnabled;
		}

		bool Scene::isAnimationEnabled() {
			return mAnimationEnabled;
		}

		bool Scene::isPhysicsEnabled() {
			return mPhysicsEnabled;
		}

		bool Scene::isInputEnabled() {
			return mInputEnabled;
		}

		void Scene::showStatus() {
			std::cout << "Scene:" << std::endl;
			std::cout << "\tUUID: " << mUUID << std::endl;
			std::cout << "\tName: " << mName << std::endl;
			std::cout << "\tAudio Enabled: "	 << isAudioEnabled() << std::endl;
			std::cout << "\tAnimation Enabled: " << isAnimationEnabled() << std::endl;
			std::cout << "\tInput Enabled: "	 << isInputEnabled() << std::endl;
			std::cout << "\tPhysics Enabled: "	 << isPhysicsEnabled() << std::endl;
		}
	}// End of Scene
} // End of Dream
