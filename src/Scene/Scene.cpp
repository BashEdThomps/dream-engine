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
		mPhysicsEnabled   = false;
		mAnimationEnabled = false;
		mInputEnabled     = false;
		mAudioEnabled     = false;
        mScriptingEnabled = false;
	}

	Scene::Scene(nlohmann::json jsonScene) {
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
            
		mScriptingEnabled = (
			jsonScene[SCENE_JSON_SCRIPTING_ENABLED].is_null() ?
            false : (bool)jsonScene[SCENE_JSON_SCRIPTING_ENABLED]
       );

		nlohmann::json sceneObjects = jsonScene[SCENE_JSON_SCENE_OBJECTS];

		if (!sceneObjects.is_null() && sceneObjects.is_array()) {
			loadSceneObjectsFromJSONArray(sceneObjects,NULL);
		}
	}

	Scene::~Scene() {
		if (mCamera != NULL) {
			delete mCamera;
		}
	}

	void Scene::init() {
		mCamera = new Camera();
		void initSceneObjects();
	}

	void Scene::initSceneObjects() {
		if (isScenegraphVectorEmpty()) {
			generateScenegraphVector();
		}
		for (std::vector<SceneObject*>::iterator sgIterator = mScenegraphVector.begin(); sgIterator != mScenegraphVector.end(); sgIterator++) {
			(*sgIterator)->init();
		}
	}
	
	bool Scene::isScenegraphVectorEmpty() {
		return mScenegraphVector.empty();
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

	void Scene::loadSceneObjectsFromJSONArray(nlohmann::json jsonArray, SceneObject* parent) {
		//std::cout << "Loading scene objects from array: "<< jsonArray.dump() << std::endl;
		if (!jsonArray.is_null()) {
			for (nlohmann::json::iterator it = jsonArray.begin(); it != jsonArray.end(); ++it) {
				std::cout << "Scene: Creating SceneObject " << std::endl;
				SceneObject *nextSceneObject = new SceneObject(*it);
				if (parent != NULL) {
					nextSceneObject->setParent(parent);
					parent->addChild(nextSceneObject);
				} else {
					setRootSceneObject(nextSceneObject);
				}
				//nextSceneObject->showStatus();
				if (!((*it)[SCENE_OBJECT_CHILDREN]).is_null()){
					loadSceneObjectsFromJSONArray((*it)[SCENE_OBJECT_CHILDREN],nextSceneObject);
				}
			}
		}
	}

	bool Scene::hasSceneObect(SceneObject *obj) {
		return mRootSceneObject == obj || mRootSceneObject->isParentOfDeep(obj);
	}

	std::string Scene::generateSceneObjectPath(SceneObject* so) {
		std::stringstream stream;
		std::vector<std::string> pathVector;

		SceneObject* next = so;
		while (next != NULL) {
			pathVector.push_back(next->getUUID());
			next = next->getParent();
		}

		std::reverse(pathVector.begin(),pathVector.end());
		for (std::vector<std::string>::iterator it = pathVector.begin(); it != pathVector.end(); ++it) {
			stream << PATH_DELIMETER << *it;
		}
		std::string retval = stream.str();
		return retval;
	}

	SceneObject* Scene::getSceneObjectByUUID(std::string uuid) {
		if (mScenegraphVector.empty()) {
			generateScenegraphVector();
		}
		for (std::vector<SceneObject*>::iterator it = mScenegraphVector.begin(); it != mScenegraphVector.end(); it++) {
		  if ((*it)->hasUUID(uuid)){
				return (*it);
			}
		}
		return NULL;
	}

	int Scene::getNumberOfSceneObjects() {
		if (mScenegraphVector.empty()) {
			generateScenegraphVector();
		}
		return mScenegraphVector.size();
	}

	void Scene::showStatus() {
		std::cout << "Scene:"                                           << std::endl;
		std::cout << "             UUID: " << mUUID                     << std::endl;
		std::cout << "             Name: " << mName                     << std::endl;
		std::cout << "    Audio Enabled: " << isAudioEnabled()          << std::endl;
		std::cout << "Animation Enabled: " << isAnimationEnabled()      << std::endl;
		std::cout << "    Input Enabled: " << isInputEnabled()          << std::endl;
		std::cout << "  Physics Enabled: " << isPhysicsEnabled()        << std::endl;
		std::cout << "    Scene Objects: " << getNumberOfSceneObjects() << std::endl;
		showScenegraph();
	}

	void Scene::showScenegraph() {
		if (mRootSceneObject == NULL) {
			std::cout << "Scenegraph is empty!" << std::endl;
			return;
		}

		generateScenegraphVector();
		generateSceneObjectPaths();
		for(std::vector<SceneObject*>::iterator it = mScenegraphVector.begin(); it != mScenegraphVector.end(); it++) {
			std::cout << (*it)->getNameUUIDString() << " @ " << (*it)->getPath() << std::endl;
		}
	}

	void Scene::setRootSceneObject(SceneObject* root) {
		mRootSceneObject = root;
	}

	SceneObject* Scene::getRootSceneObject() {
		return mRootSceneObject;
	}

	void Scene::generateScenegraphVector() {
		mScenegraphVector.clear();
		mRootSceneObject->getChildrenVectorDeep(&mScenegraphVector);
	}

	void Scene::generateSceneObjectPaths() {
		if (mScenegraphVector.empty()) {
			generateScenegraphVector();
		}
		for(std::vector<SceneObject*>::iterator it = mScenegraphVector.begin(); it != mScenegraphVector.end(); it++) {
			 (*it)->generatePath();
		}
	}
		
	}// End of Scene
} // End of Dream
