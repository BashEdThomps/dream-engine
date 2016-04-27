/*
* Dream::Scene::Scene
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
			mUUID = jsonScene[SCENE_JSON_UUID];
			mName = jsonScene[SCENE_JSON_NAME];

			mPhysicsEnabled = (
				jsonScene[SCENE_JSON_PHYSICS_ENABLED].is_null() ?
				false : (bool)jsonScene[SCENE_JSON_PHYSICS_ENABLED]
			);

			mAnimationEnabled = (
				jsonScene[SCENE_JSON_ANIMATION_ENABLED].is_null() ?
				false : (bool)jsonScene[SCENE_JSON_ANIMATION_ENABLED]
			);

			mInputEnabled = (
				jsonScene[SCENE_JSON_INPUT_ENABLED].is_null() ?
				false :(bool)jsonScene[SCENE_JSON_INPUT_ENABLED]
			);

			mAudioEnabled = (
				jsonScene[SCENE_JSON_AUDIO_ENABLED].is_null() ?
				false : (bool)jsonScene[SCENE_JSON_AUDIO_ENABLED]
			);

			nlohmann::json sceneObjects = jsonScene[SCENE_JSON_SCENE_OBJECTS];

			if (!sceneObjects.is_null() && sceneObjects.is_array()) {
				loadSceneObjectsFromJSONArray(sceneObjects);
			}
		}

		Scene::~Scene() {

		}

		void Scene::init() {
			mCamera = new Camera();
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

		void Scene::loadSceneObjectsFromJSONArray(nlohmann::json jsonArray) {
			for (nlohmann::json::iterator it = jsonArray.begin(); it != jsonArray.end(); ++it) {
				std::cout << "Scene: Creating SceneObject" << std::endl;
				SceneObject *nextSceneObject = new SceneObject(*it);
				nextSceneObject->showStatus();
				addSceneObject(nextSceneObject);
			}
		}

		bool Scene::hasSceneObect(SceneObject *obj) {
			return std::find(mSceneObjects.begin(), mSceneObjects.end(), obj) != mSceneObjects.end();
		}

		void Scene::addSceneObject(SceneObject* so) {
			mSceneObjects.push_back(so);
		}

		void Scene::removeSceneObject(SceneObject* so) {
			if (so == NULL) {
				return;
			}
			mSceneObjects.erase(std::remove(mSceneObjects.begin(), mSceneObjects.end(), so), mSceneObjects.end());
			return;
		}

		std::string Scene::generateSceneObjectPath(SceneObject* so) {
			std::stringstream stream;
			std::vector<std::string> pathVector;

			SceneObject* next = so;
			while (next != NULL) {
				pathVector.push_back(next->getUUID());
				next = getSceneObjectByUUID(next->getParentUUID());
			}

			std::reverse(pathVector.begin(),pathVector.end());
			for (std::vector<std::string>::iterator it = pathVector.begin();
					 it != pathVector.end(); ++it) {
					 stream << PATH_DELIMETER << *it;
			}
			std::string retval = stream.str();
			return retval;
		}

		SceneObject* Scene::getSceneObjectByName(std::string name) {
			for (std::vector<SceneObject*>::iterator next = mSceneObjects.begin(); next != mSceneObjects.end(); ++next) {
				if ((*next)->hasName(name)) {
					return (*next);
				}
			}
			return NULL;
		}

		SceneObject* Scene::getSceneObjectByUUID(std::string uuid) {
			for (std::vector<SceneObject*>::iterator next = mSceneObjects.begin(); next != mSceneObjects.end(); ++next) {
				if ((*next)->hasUUID(uuid)) {
					return (*next);
				}
			}
			return NULL;
		}

		int Scene::getNumberOfSceneObjects() {
			return mSceneObjects.size();
		}

		int Scene::countChildrenOfSceneObject(SceneObject* obj) {
			int count = 0;
			for (std::vector<SceneObject*>::iterator next = mSceneObjects.begin(); next != mSceneObjects.end(); ++next) {
				if ((*next)->getParentUUID().compare(obj->getUUID()) == 0) {
					count++;
				}
			}
			return count;
		}

		void Scene::showStatus() {
			std::cout << "Scene:"                                               << std::endl;
			std::cout << "             UUID: " << mUUID                     << std::endl;
			std::cout << "             Name: " << mName                     << std::endl;
			std::cout << "    Audio Enabled: " << isAudioEnabled()          << std::endl;
			std::cout << "Animation Enabled: " << isAnimationEnabled()      << std::endl;
			std::cout << "    Input Enabled: " << isInputEnabled()          << std::endl;
			std::cout << "  Physics Enabled: " << isPhysicsEnabled()        << std::endl;
			std::cout << "    Scene Objects: " << getNumberOfSceneObjects() << std::endl;
		}
	}// End of Scene
} // End of Dream
